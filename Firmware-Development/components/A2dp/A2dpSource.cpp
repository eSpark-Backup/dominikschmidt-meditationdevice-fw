/* Play music from Bluetooth device

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "A2dpSource.hpp"
#include "freertos/semphr.h"

xSemaphoreHandle xAudioMutex;

const char *file_path = "/sdcard/audio/Waves 1.wav"; // MP3 Or WAV
const char *file_path2 = "/sdcard/audio/Thunder 1.wav"; // MP3 Or WAV
const char *file_path3 = "/sdcard/audio/Garden Rain 1.mp3"; // MP3 Or WAV
const char *file_path4 = "/sdcard/audio/Stream 1.wav"; // MP3 Or WAV
const char *file_path5 = "/sdcard/audio/Forest Rain 1.mp3"; // MP3 Or WAV
const char *file_path6 = "/sdcard/audio/Birds 1.wav"; // MP3 Or WAV
int audioCount = 1;
char filePath[256];
typedef uint8_t esp_peer_bdname_t[ESP_BT_GAP_MAX_BDNAME_LEN + 1];

static const char *TAG = "BLUETOOTH_A2DP";
bool deviceConnected = false;

int64_t play_time_us = 0;
int64_t start_time_us = 0;
#define MAX_SCANNED_DEVICES 6

typedef struct {
    char name[ESP_BT_GAP_MAX_BDNAME_LEN + 1];
    esp_bd_addr_t bda;
} ScannedDevice;

static ScannedDevice scanned_devices[MAX_SCANNED_DEVICES];
static int scanned_device_count = 0;

audio_pipeline_handle_t pipeline;
audio_element_handle_t fatfs_stream_reader, bt_stream_writer, music_decoder;
mp3_decoder_cfg_t mp3_cfg;
wav_decoder_cfg_t wav_cfg;
esp_periph_set_handle_t set;
esp_periph_handle_t bt_periph;
audio_event_iface_handle_t evt;
TaskHandle_t xBtTaskHandler;
bool AudioPlaying = false;
A2DP btAudio;

void list_sdcard_files(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        printf("Failed to open directory: %s\n", path);
        return;
    }

    struct dirent *entry;
    printf("Listing files in: %s\n", path);

    while ((entry = readdir(dir)) != NULL) {
        printf( "File: %s\n", entry->d_name);
    }

    closedir(dir);
}

void A2DP::print_bonded_devices()
{
    int num_bonded = 15;
    esp_bd_addr_t dev_list[10];

    esp_err_t res = esp_bt_gap_get_bond_device_list(&num_bonded, dev_list);

    if (res == ESP_OK) {
        printf("Number of bonded devices: %d\n", num_bonded);
        for (int i = 0; i < num_bonded; i++) {
            printf("Device %d MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", i,
                   dev_list[i][0], dev_list[i][1], dev_list[i][2], 
                   dev_list[i][3], dev_list[i][4], dev_list[i][5]);
                if (esp_bt_gap_remove_bond_device(dev_list[i]) == ESP_OK) {
                    printf("Removed bonded device %d\n", i);
                } else {
                    printf("Failed to remove bonded device %d\n", i);
                }
        }
    } else if (res == ESP_ERR_NOT_FOUND) {
        printf("No bonded devices found.\n");
    } else {
        printf("Error retrieving bonded devices: %d\n", res);
    }
}

char *A2DP::bda2str(esp_bd_addr_t bda, char *str, size_t size)
{
    if (bda == NULL || str == NULL || size < 18) {
        return NULL;
    }

    uint8_t *p = bda;
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            p[0], p[1], p[2], p[3], p[4], p[5]);
    return str;
}

bool A2DP::get_name_from_eir(uint8_t *eir, uint8_t *bdname, uint8_t *bdname_len)
{
    uint8_t *rmt_bdname = NULL;
    uint8_t rmt_bdname_len = 0;

    if (!eir) {
        return false;
    }

    rmt_bdname = esp_bt_gap_resolve_eir_data(eir, ESP_BT_EIR_TYPE_CMPL_LOCAL_NAME, &rmt_bdname_len);
    if (!rmt_bdname) {
        rmt_bdname = esp_bt_gap_resolve_eir_data(eir, ESP_BT_EIR_TYPE_SHORT_LOCAL_NAME, &rmt_bdname_len);
    }

    if (rmt_bdname) {
        if (rmt_bdname_len > ESP_BT_GAP_MAX_BDNAME_LEN) {
            rmt_bdname_len = ESP_BT_GAP_MAX_BDNAME_LEN;
        }

        if (bdname) {
            memcpy(bdname, rmt_bdname, rmt_bdname_len);
            bdname[rmt_bdname_len] = '\0';
        }
        if (bdname_len) {
            *bdname_len = rmt_bdname_len;
        }
        return true;
    }

    return false;
}

void A2DP::filter_inquiry_scan_result(esp_bt_gap_cb_param_t *param) {
    if (scanned_device_count >= MAX_SCANNED_DEVICES) return;  // Stop if array is full

    char bda_str[18];
    esp_peer_bdname_t peer_bdname;
    uint8_t *eir = NULL;
    esp_bt_gap_dev_prop_t *p;
    uint32_t cod = 0;
    int32_t rssi = -129; /* invalid value */
    bool name_found = false;

    ESP_LOGI(TAG, "Scanned device: %s", bda2str(param->disc_res.bda, bda_str, 18));

    // Check if device already exists
    for (int i = 0; i < scanned_device_count; i++) {
        if (memcmp(scanned_devices[i].bda, param->disc_res.bda, ESP_BD_ADDR_LEN) == 0) {
            ESP_LOGI(TAG, "Device %s already scanned, skipping.", bda_str);
            return;  // ✅ Skip duplicate device
        }
    }

    // Extract device properties
    for (int i = 0; i < param->disc_res.num_prop; i++) {
        p = param->disc_res.prop + i;
        switch (p->type) {
            case ESP_BT_GAP_DEV_PROP_COD:
                cod = *(uint32_t *)(p->val);
                ESP_LOGI(TAG, "--Class of Device: 0x%" PRIx32, cod);
                break;
            case ESP_BT_GAP_DEV_PROP_RSSI:
                rssi = *(int8_t *)(p->val);
                ESP_LOGI(TAG, "--RSSI: %" PRId32, rssi);
                break;
            case ESP_BT_GAP_DEV_PROP_EIR:
                eir = (uint8_t *)(p->val);
                if (get_name_from_eir(eir, (uint8_t *)&peer_bdname, NULL)) {
                    name_found = true;
                    ESP_LOGI(TAG, "--Name: %s", peer_bdname);
                }
                break;
            case ESP_BT_GAP_DEV_PROP_BDNAME:
                // Alternative method to extract name
                strncpy((char *)peer_bdname, (char *)p->val, ESP_BT_GAP_MAX_BDNAME_LEN);
                peer_bdname[ESP_BT_GAP_MAX_BDNAME_LEN] = '\0';
                name_found = true;
                ESP_LOGI(TAG, "--Name (BDNAME): %s", peer_bdname);
                break;
            default:
                break;
        }
    }

    // ✅ Store device only if it has a name
    if (name_found && scanned_device_count < MAX_SCANNED_DEVICES) {
        strncpy(scanned_devices[scanned_device_count].name, (char *)peer_bdname, ESP_BT_GAP_MAX_BDNAME_LEN);
        memcpy(scanned_devices[scanned_device_count].bda, param->disc_res.bda, ESP_BD_ADDR_LEN);
        scanned_device_count++;
        ESP_LOGI(TAG, "✅ Stored device: %s", peer_bdname);
    } else {
        ESP_LOGW(TAG, "⚠ Skipping device without name.");
    }
}

void A2DP::bt_app_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
    switch (event) {
        case ESP_BT_GAP_DISC_RES_EVT: {
                filter_inquiry_scan_result(param);
                break;
            }
        case ESP_BT_GAP_DISC_STATE_CHANGED_EVT: {
            if (param->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STOPPED) {
                ESP_LOGI(TAG, "Device discovery stopped. Found %d devices:", scanned_device_count);
                for (int i = 0; i < scanned_device_count; i++) {
                    ESP_LOGI(TAG, "[%d] Name: %s", i + 1, scanned_devices[i].name);
                }
                update_bt_dropdown(ui_ScannedDevDropdown);
            } else if (param->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STARTED) {
                ESP_LOGI(TAG, "Discovery started.");
                scanned_device_count = 0;  // Reset list on new scan
            }
            break;
            }
        case ESP_BT_GAP_PIN_REQ_EVT: {
                // Fix: Wrap the declaration inside braces
            ESP_LOGI(TAG, "ESP_BT_GAP_PIN_REQ_EVT min_16_digit:%d", param->pin_req.min_16_digit);
            esp_bt_pin_code_t pin_code = {'1', '2', '3', '4'};
            esp_bt_gap_pin_reply(param->pin_req.bda, true, 4, pin_code);
            break;
            }
        case ESP_BT_GAP_ACL_CONN_CMPL_STAT_EVT: {
            esp_bd_addr_t connected_bda;
            memcpy(connected_bda, param->acl_conn_cmpl_stat.bda, sizeof(esp_bd_addr_t));

            printf( "Device Connected - MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                        connected_bda[0], connected_bda[1], connected_bda[2], 
                        connected_bda[3], connected_bda[4], connected_bda[5]);

            // Get device name
            if (esp_bt_gap_read_remote_name(connected_bda) == ESP_OK) {
                printf("Fetching remote device name...\n");
            } else {
                printf("Failed to fetch remote device name\n");
            }
            break;
        }
        case ESP_BT_GAP_ACL_DISCONN_CMPL_STAT_EVT: {
            esp_bd_addr_t disconnected_bda;
            memcpy(disconnected_bda, param->acl_disconn_cmpl_stat.bda, sizeof(esp_bd_addr_t));

            printf("Device Disconnected - MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                        disconnected_bda[0], disconnected_bda[1], disconnected_bda[2], 
                        disconnected_bda[3], disconnected_bda[4], disconnected_bda[5]);
            break;        
        }
        case ESP_BT_GAP_READ_REMOTE_NAME_EVT: { // Device Name Received
            if (param->read_rmt_name.stat == ESP_BT_STATUS_SUCCESS) {
                printf("Connected Device Name: %s\n", param->read_rmt_name.rmt_name);
                deviceConnected = true;
            } else {
                printf("Failed to get remote device name\n");
                deviceConnected = false;
            }
            break;
        }
        default:
            break;
    }
    return;
}

/* Starts Bluetooth scanning */
void A2DP::start_bt_scan() {

    ESP_LOGI(TAG, "Starting Bluetooth scan...");
    scanned_device_count = 0;  // Reset scanned devices count
    memset(scanned_devices, 0, sizeof(scanned_devices));
    esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 5, 0);
    esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
}

/* Connects to a Bluetooth device by name */
bool A2DP::connect_to_bt_device(const char *device_name)
{
    for (int i = 0; i < scanned_device_count; i++) {
        if (strcmp(scanned_devices[i].name, device_name) == 0) {
            char bda_str[18];
            ESP_LOGI(TAG, "Connecting to %s (%s)...", device_name, bda2str(scanned_devices[i].bda, bda_str, 18));
            if (esp_a2d_source_connect(scanned_devices[i].bda) == ESP_OK) {
                ESP_LOGI(TAG, "Connection successful.");
                deviceConnected = true;
                return true;
            } else {
                ESP_LOGE(TAG, "Connection failed.");
                deviceConnected = false;
                return false;
            }
        }
    }
    memset(deviceName, 0, BUF_SIZE);
    ESP_LOGW(TAG, "Device %s not found.", device_name);
    return false;
}

void A2DP::A2dpServiceInit()
{
    ESP_LOGI(TAG, "[ 2 ] Bluetooth setup");
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT));
    ESP_ERROR_CHECK(esp_bluedroid_init());
    ESP_ERROR_CHECK(esp_bluedroid_enable());
    print_bonded_devices();

    esp_bt_dev_set_device_name("ESP_DOMINIK");
    esp_bt_pin_type_t pin_type = ESP_BT_PIN_TYPE_FIXED;
    esp_bt_pin_code_t pin_code = {'1', '2', '3', '4'};
    esp_bt_gap_set_pin(pin_type, 4, pin_code);
    esp_bt_gap_register_callback(bt_app_gap_cb);

    ESP_LOGI(TAG, "[ 4 ] Create audio pipeline");
    audio_pipeline_cfg_t pipeline_cfg = DEFAULT_AUDIO_PIPELINE_CONFIG();
    pipeline = audio_pipeline_init(&pipeline_cfg);
    mem_assert(pipeline);
    
    fatfs_stream_cfg_t fatfs_cfg = FATFS_STREAM_CFG_DEFAULT();
    fatfs_cfg.type = AUDIO_STREAM_READER;
    fatfs_stream_reader = fatfs_stream_init(&fatfs_cfg);

    a2dp_stream_config_t a2dp_config = {
        .type = AUDIO_STREAM_WRITER,
        .user_callback = {0},
    };
    bt_stream_writer = a2dp_stream_init(&a2dp_config);
    
    if (strstr(file_path, ".mp3")) {
        mp3_cfg = DEFAULT_MP3_DECODER_CONFIG();
        music_decoder = mp3_decoder_init(&mp3_cfg);
    } else {
        wav_cfg = DEFAULT_WAV_DECODER_CONFIG();
        music_decoder = wav_decoder_init(&wav_cfg);
    }
    mem_assert(music_decoder);
    audio_element_set_uri(fatfs_stream_reader, file_path);

    ESP_LOGI(TAG, "[ 5 ] Register pipeline elements");
    audio_pipeline_register(pipeline, fatfs_stream_reader, "file");
    audio_pipeline_register(pipeline, music_decoder, "dec");
    audio_pipeline_register(pipeline, bt_stream_writer, "bt");
    const char *link_tag[3] = {"file", "dec", "bt"};
    audio_pipeline_link(pipeline, &link_tag[0], 3);
            
     
    ESP_LOGI(TAG, "[ 3 ] Create bt peripheral");
    bt_periph = bt_create_periph();
    esp_periph_start(set, bt_periph);
    
    ESP_LOGI(TAG, "[ 6 ] Set up event listener");
    audio_event_iface_cfg_t evt_cfg = AUDIO_EVENT_IFACE_DEFAULT_CFG();
    evt = audio_event_iface_init(&evt_cfg);
    audio_pipeline_set_listener(pipeline, evt);
    audio_event_iface_set_listener(esp_periph_set_get_event_iface(set), evt);

    xAudioMutex = xSemaphoreCreateMutex();
}

void A2DP::A2dpAudioStart(char *filepath, int play_time_sec) 
{
    strncpy(filePath, filepath, sizeof(filePath) - 1);
    filePath[sizeof(filePath) - 1] = '\0';  // ensure null-termination

    static char last_file_ext[5] = "";  // holds ".mp3" or ".wav"
    char current_file_ext[5] = "";

    // Determine current file extension
    if (strstr(filepath, ".mp3")) {
        strcpy(current_file_ext, ".mp3");
    } else if (strstr(filepath, ".wav")) {
        strcpy(current_file_ext, ".wav");
    }

    ESP_LOGW(TAG, "[Restarting] Setting new file: %s", filepath);
    if (xSemaphoreTake(xAudioMutex, pdMS_TO_TICKS(1000)))
    {
        if (AudioPlaying) A2dpAudioStop();

        // Only reset decoder if file format has changed
        if (strcmp(current_file_ext, last_file_ext) != 0)
        {
            ESP_LOGI(TAG, "Changing decoder from %s to %s", last_file_ext, current_file_ext);

            audio_pipeline_unregister(pipeline, fatfs_stream_reader);
            audio_pipeline_unregister(pipeline, music_decoder);
            audio_pipeline_unregister(pipeline, bt_stream_writer);
            audio_element_reset_state(fatfs_stream_reader);
            audio_element_reset_state(music_decoder);
            audio_element_reset_state(bt_stream_writer);

            // Optional: Deinit decoder if switching between WAV <-> MP3
            if (music_decoder) {
                audio_element_deinit(music_decoder);
                music_decoder = NULL;
            }

            // Re-init decoder based on new file type
            if (strcmp(current_file_ext, ".mp3") == 0) {
                music_decoder = mp3_decoder_init(&mp3_cfg);
            } else {
                music_decoder = wav_decoder_init(&wav_cfg);
            }

            strcpy(last_file_ext, current_file_ext);  // update last file extension
            
            // Always update file source
            audio_pipeline_register(pipeline, fatfs_stream_reader, "file");
            audio_pipeline_register(pipeline, music_decoder, "dec");
            audio_pipeline_register(pipeline, bt_stream_writer, "bt");
        }

        
        audio_element_set_uri(fatfs_stream_reader, filepath);

        const char *link_tag[3] = {"file", "dec", "bt"};
        audio_pipeline_link(pipeline, &link_tag[0], 3);
        audio_pipeline_run(pipeline);

        AudioPlaying = true;

        if (play_time_sec > 10)
        {
            setTimerValues(play_time_sec, true);
            play_time_us = (int64_t)play_time_sec * 1000;
            xTaskCreatePinnedToCore(loopAudioTask, "Audio_Monitor", 4096, NULL, 4, &xBtTaskHandler, 0);
        }
        ESP_LOGI(TAG, "loopAudioTask exit");
        xSemaphoreGive(xAudioMutex);
    }
}

void A2DP::loopAudioTask(void *param)
{
    ESP_LOGI(TAG, "A2dpAudioStart started");
    audio_event_iface_msg_t msg;
    esp_err_t ret;
    unsigned long swTime = xTaskGetTickCount();
    start_time_us = xTaskGetTickCount();
    static int lastCount = audioCount;
    // loadAudio();
    ESP_LOGW(TAG, "[Starting] Playback time %u : %lld : %lld",xTaskGetTickCount(), start_time_us, play_time_us);
    while (AudioPlaying) 
    {
        if (xSemaphoreTake(xAudioMutex, pdMS_TO_TICKS(100)))
        {
            // if (lastCount != audioCount) {

            //     ESP_LOGW(TAG, "[Audio Switch] Stopping pipeline...");
            //     audio_pipeline_stop(pipeline);
            //     audio_pipeline_wait_for_stop(pipeline);
            //     audio_pipeline_terminate(pipeline);
            
            //     // Unregister all existing elements
            //     audio_pipeline_unregister(pipeline, fatfs_stream_reader);
            //     audio_pipeline_unregister(pipeline, music_decoder);
            //     audio_pipeline_unregister(pipeline, bt_stream_writer);
            
            //     // Reset their states
            //     audio_element_reset_state(fatfs_stream_reader);
            //     audio_element_reset_state(music_decoder);
            //     audio_element_reset_state(bt_stream_writer);
            
            //     // Optional: Deinit decoder if switching between WAV <-> MP3
            //     if (music_decoder) {
            //         audio_element_deinit(music_decoder);
            //         music_decoder = NULL;
            //     }
            
            //     // Choose decoder & file path based on audioCount
            //     switch (audioCount) {
            //         case 1:
            //             music_decoder = wav_decoder_init(&wav_cfg);
            //             strcpy(filePath, file_path);
            //             break;
            //         case 2:
            //             music_decoder = wav_decoder_init(&wav_cfg);
            //             strcpy(filePath, file_path2);
            //             break;
            //         case 3:
            //             music_decoder = mp3_decoder_init(&mp3_cfg);
            //             strcpy(filePath, file_path3);
            //             break;
            //         case 4:
            //             music_decoder = wav_decoder_init(&wav_cfg);
            //             strcpy(filePath, file_path4);
            //             break;
            //         case 5:
            //             music_decoder = mp3_decoder_init(&mp3_cfg);
            //             strcpy(filePath, file_path5);
            //             break;
            //         case 6:
            //             music_decoder = wav_decoder_init(&wav_cfg);
            //             strcpy(filePath, file_path6);
            //             break;
            //         default:
            //             ESP_LOGE(TAG, "Invalid audioCount: %d", audioCount);
            //             break;
            //     }
            
            //     ESP_LOGI(TAG, "[Audio Switch] Using file: %s", filePath);
            
            //     // Register updated elements back into the pipeline
            //     audio_pipeline_register(pipeline, fatfs_stream_reader, "file");
            //     audio_pipeline_register(pipeline, music_decoder, "dec");
            //     audio_pipeline_register(pipeline, bt_stream_writer, "bt");
            
            //     // Set new file path
            //     audio_element_set_uri(fatfs_stream_reader, filePath);
            
            //     // Re-link pipeline
            //     const char *link_tag[3] = {"file", "dec", "bt"};
            //     audio_pipeline_link(pipeline, &link_tag[0], 3);
            
            //     // Start the pipeline
            //     ESP_LOGW(TAG, "[Audio Switch] Restarting pipeline...");
            //     audio_pipeline_run(pipeline);
            
            //     lastCount = audioCount;
            // }
            if ((int64_t)(xTaskGetTickCount() - start_time_us) >= play_time_us) 
            {
                ESP_LOGW(TAG, "[Stopping] Playback time reached %u : %lld : %lld",xTaskGetTickCount(), start_time_us, play_time_us);
                AudioPlaying = false;
                
                audio_pipeline_stop(pipeline);
                audio_pipeline_wait_for_stop(pipeline);
                audio_pipeline_terminate(pipeline);
                audio_pipeline_reset_ringbuffer(pipeline);
                audio_pipeline_reset_elements(pipeline);
                audio_pipeline_reset_items_state(pipeline);
                xSemaphoreGive(xAudioMutex);
                break;
            }
            
            ret = audio_event_iface_listen(evt, &msg, pdMS_TO_TICKS(100));

            static int lastData = 100;  static int lastCmd = 100;
            if (lastData != (int)msg.data || lastCmd != msg.cmd) {
                ESP_LOGW(TAG, "[msg event] source: %d lastData: %d msg.data: %d lastCmd: %d msg.cmd: %d",msg.source_type, lastData, (int)msg.data, lastCmd, msg.cmd);
                lastData = (int)msg.data; lastCmd = msg.cmd;
            }
            if (ret == ESP_OK && msg.source_type == AUDIO_ELEMENT_TYPE_ELEMENT && msg.cmd == AEL_MSG_CMD_REPORT_STATUS
                && (((int)msg.data == AEL_STATUS_STATE_FINISHED))) {
                    // ESP_LOGE(TAG, "[ LOOP ] LOOPING");
                    ESP_LOGW(TAG, "[ LOOP ] Playback time %u : %lld : %lld",xTaskGetTickCount(), start_time_us, play_time_us);
                audio_pipeline_stop(pipeline);
                audio_pipeline_wait_for_stop(pipeline);
                audio_pipeline_terminate(pipeline);
                audio_pipeline_reset_ringbuffer(pipeline);
                audio_pipeline_reset_elements(pipeline);
                audio_pipeline_reset_items_state(pipeline);
                audio_pipeline_run(pipeline);
            }
            // vTaskDelay(pdMS_TO_TICKS(50));
            xSemaphoreGive(xAudioMutex);
        }
    }
    // Cleanup: Delete the task properly
    vTaskDelete(NULL);
}

void A2DP::mountSDcard()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    
    ESP_LOGI(TAG, "[ 1 ] Mount sdcard");
    esp_periph_config_t periph_cfg = DEFAULT_ESP_PERIPH_SET_CONFIG();
    set = esp_periph_set_init(&periph_cfg);
    audio_board_sdcard_init(set, SD_MODE_SPI);
}

bool A2DP::isAudioStarted()
{
    return AudioPlaying;
}

void A2DP::A2dpAudioStop()
{
    ESP_LOGI(TAG, "[ 7 ] Stop audio_pipeline");
    if (xBtTaskHandler != NULL) {
        vTaskDelete(xBtTaskHandler);
        xBtTaskHandler = NULL;
    }
    AudioPlaying = false;

    audio_pipeline_stop(pipeline);
    audio_pipeline_wait_for_stop(pipeline);
    audio_pipeline_terminate(pipeline);
//     audio_pipeline_reset_ringbuffer(pipeline);
//     audio_pipeline_reset_elements(pipeline);
//     audio_pipeline_reset_items_state(pipeline);
}

void A2DP::update_bt_dropdown(lv_obj_t *dropdown)
{
    if (scanned_device_count == 0) {
        ESP_LOGI(TAG, "No devices found.");
        lv_dropdown_set_options(dropdown, "No Devices Found");
        return;
    }

    size_t total_length = 13; // Start with 1 for null terminator + "Select None"
    for (int i = 0; i < scanned_device_count; i++) {
        total_length += strlen(scanned_devices[i].name) + 1;
    }

    char *dropdown_options = (char *)malloc(total_length);
    if (dropdown_options == NULL) {
        ESP_LOGE(TAG, "Memory allocation failed!");
        return;
    }

    dropdown_options[0] = '\0';

    ESP_LOGI(TAG, "Final device list for dropdown:");
    strcat(dropdown_options, "Select None\n");
    for (int i = 0; i < scanned_device_count; i++) {
        strcat(dropdown_options, scanned_devices[i].name);
        if (i < scanned_device_count - 1) strcat(dropdown_options, "\n");
        ESP_LOGI(TAG, "[%d] %s", i + 1, scanned_devices[i].name);
    }

    ESP_LOGI(TAG, "Dropdown string: %s", dropdown_options);

    // ✅ Debug log before setting options
    ESP_LOGI(TAG, "Updating dropdown with new options...");

    lv_dropdown_set_options(ui_ScannedDevDropdown, dropdown_options);

    // ✅ Force refresh the dropdown UI
    lv_obj_invalidate(dropdown);
    lv_obj_refresh_style(dropdown, LV_PART_MAIN, LV_STATE_DEFAULT);

    free(dropdown_options);
    lv_dropdown_set_text(ui_ScannedDevDropdown, "Select Device");
}

void A2DP::update_dropdown_options(lv_obj_t *dropdown, char **options, int option_count)
{
    // Calculate required buffer size
    size_t total_length = 1; // Start with 1 for null terminator

    for (int i = 0; i < option_count; i++) {
        total_length += strlen(options[i]) + 1; // +1 for '\n'
    }

    // Allocate memory
    char *option_str = (char *)malloc(total_length);
    if (option_str == NULL) return; // Handle memory allocation failure

    option_str[0] = '\0'; // Initialize empty string

    // Build the options string
    for (int i = 0; i < option_count; i++) {
        strcat(option_str, options[i]);
        if (i < option_count - 1) strcat(option_str, "\n"); // Add '\n' between options
    }

    // Set dropdown options
    lv_dropdown_set_options(dropdown, option_str);

    // Free allocated memory
    free(option_str);
}
