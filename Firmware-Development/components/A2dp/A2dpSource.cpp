/* Play music from Bluetooth device

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "A2dpSource.hpp"


#define BT_CONNECT_TIMEOUT      20000
const char *file_path = "/sdcard/converted/Birds 1.wav"; // MP3 Or WAV

typedef uint8_t esp_peer_bdname_t[ESP_BT_GAP_MAX_BDNAME_LEN + 1];

static const char *TAG = "BLUETOOTH_A2DP";
static esp_peer_bdname_t remote_bt_device_name;
static bool device_found = false;
static esp_bd_addr_t remote_bd_addr = {0};

#define MOUNT_POINT "/sdcard/sdcard"
audio_pipeline_handle_t pipeline;
audio_element_handle_t fatfs_stream_reader, bt_stream_writer, music_decoder;
esp_periph_set_handle_t set;
esp_periph_handle_t bt_periph;
audio_event_iface_handle_t evt;
TaskHandle_t xBtTaskHandler;
bool AudioPlaying = false;


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

static char *bda2str(esp_bd_addr_t bda, char *str, size_t size)
{
    if (bda == NULL || str == NULL || size < 18) {
        return NULL;
    }

    uint8_t *p = bda;
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            p[0], p[1], p[2], p[3], p[4], p[5]);
    return str;
}

static bool get_name_from_eir(uint8_t *eir, uint8_t *bdname, uint8_t *bdname_len)
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

static void filter_inquiry_scan_result(esp_bt_gap_cb_param_t *param)
{
    char bda_str[18];
    uint32_t cod = 0;
    int32_t rssi = -129; /* invalid value */
    uint8_t *eir = NULL;
    esp_peer_bdname_t peer_bdname;
    esp_bt_gap_dev_prop_t *p;

    ESP_LOGI(TAG, "Scanned device: %s", bda2str(param->disc_res.bda, bda_str, 18));
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
                get_name_from_eir(eir, (uint8_t *)&peer_bdname, NULL);
                ESP_LOGI(TAG, "--Name: %s", peer_bdname);
                break;
            case ESP_BT_GAP_DEV_PROP_BDNAME:
            default:
                break;
        }
    }

    ESP_LOGI(TAG, "need device name %s", (uint8_t *)remote_bt_device_name);
    /* search for device named "peer_bdname" in its extended inquiry response */
    if (eir) {
        get_name_from_eir(eir, (uint8_t *)&peer_bdname, NULL);
        if (strcmp((char *)peer_bdname, (char *)remote_bt_device_name) != 0) {
            return;
        }

        ESP_LOGI(TAG, "Found a target device, address %s, name %s", bda_str, (uint8_t *)peer_bdname);
        device_found = true;
        memcpy(&remote_bd_addr, param->disc_res.bda, ESP_BD_ADDR_LEN);
        ESP_LOGI(TAG, "Cancel device discovery ...");
        esp_bt_gap_cancel_discovery();
    }
}

static void bt_app_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
    switch (event) {
        case ESP_BT_GAP_DISC_RES_EVT: {
                filter_inquiry_scan_result(param);
                break;
            }
        case ESP_BT_GAP_DISC_STATE_CHANGED_EVT: {
                if (param->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STOPPED) {
                    if (device_found) {
                        ESP_LOGI(TAG, "Device discovery stopped.");
                        ESP_LOGI(TAG, "a2dp connecting to peer: %s", remote_bt_device_name);
                        device_found = false;
                        esp_a2d_source_connect(remote_bd_addr);
                    } else {
                        // not discovered, continue to discover
                        ESP_LOGI(TAG, "Device discovery failed, continue to discover...");
                        esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);
                    }
                } else if (param->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STARTED) {
                    ESP_LOGI(TAG, "Discovery started.");
                }
                break;
            }
        case ESP_BT_GAP_PIN_REQ_EVT: {
                ESP_LOGI(TAG, "ESP_BT_GAP_PIN_REQ_EVT min_16_digit:%d", param->pin_req.min_16_digit);
                if (param->pin_req.min_16_digit) {
                    ESP_LOGI(TAG, "Input pin code: 0000 0000 0000 0000");
                    esp_bt_pin_code_t pin_code = {0};
                    esp_bt_gap_pin_reply(param->pin_req.bda, true, 16, pin_code);
                } else {
                    esp_bt_pin_code_t pin_code = {'1', '2', '3', '4'};
                    esp_bt_gap_pin_reply(param->pin_req.bda, true, 4, pin_code);
                }
                break;
            }
        default:
            break;
    }
    return;
}


void A2dpServiceInit()
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

    ESP_LOGI(TAG, "[ 2 ] Bluetooth setup");
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT));
    ESP_ERROR_CHECK(esp_bluedroid_init());
    ESP_ERROR_CHECK(esp_bluedroid_enable());

    esp_bt_dev_set_device_name("ESP_SOURCE_STREAM_DEMO");
    esp_bt_gap_register_callback(bt_app_gap_cb);
    
    ESP_LOGI(TAG, "[ 3 ] Create bt peripheral");
    bt_periph = bt_create_periph();
    esp_periph_start(set, bt_periph);
}

void A2dpAudioStart(char *filepath)
{
    ESP_LOGI(TAG, "[ 4 ] Create audio pipeline");
    audio_pipeline_cfg_t pipeline_cfg = DEFAULT_AUDIO_PIPELINE_CONFIG();
    pipeline = audio_pipeline_init(&pipeline_cfg);
    mem_assert(pipeline);

    fatfs_stream_cfg_t fatfs_cfg = FATFS_STREAM_CFG_DEFAULT();
    fatfs_cfg.type = AUDIO_STREAM_READER;
    fatfs_stream_reader = fatfs_stream_init(&fatfs_cfg);
    
    if (strstr(filepath, ".mp3")) {
        mp3_decoder_cfg_t mp3_cfg = DEFAULT_MP3_DECODER_CONFIG();
        music_decoder = mp3_decoder_init(&mp3_cfg);
    } else if (strstr(filepath, ".wav")) {
        wav_decoder_cfg_t wav_cfg = DEFAULT_WAV_DECODER_CONFIG();
        music_decoder = wav_decoder_init(&wav_cfg);
    } else {
        ESP_LOGE(TAG, "Unsupported file format!");
        return;
    }
    mem_assert(music_decoder);


    const char *remote_name = NULL;
    remote_name = "RONIN (R-140)"; // CONFIG_BT_REMOTE_NAME;
    if (remote_name) {
        memcpy(&remote_bt_device_name, remote_name, strlen(remote_name) + 1);
    } else {
        memcpy(&remote_bt_device_name, "RONIN (R-140)", ESP_BT_GAP_MAX_BDNAME_LEN);
    }

    a2dp_stream_config_t a2dp_config = {
        .type = AUDIO_STREAM_WRITER,
        .user_callback = {0},
    };
    bt_stream_writer = a2dp_stream_init(&a2dp_config);

    esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);
    esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);


    ESP_LOGI(TAG, "[ 5 ] Register pipeline elements");
    audio_pipeline_register(pipeline, fatfs_stream_reader, "file");
    audio_pipeline_register(pipeline, music_decoder, "dec");
    audio_pipeline_register(pipeline, bt_stream_writer, "bt");
    
    const char *link_tag[3] = {"file", "dec", "bt"};
    audio_pipeline_link(pipeline, &link_tag[0], 3);
    
    audio_element_set_uri(fatfs_stream_reader, filepath);
    
    ESP_LOGI(TAG, "[ 6 ] Start audio pipeline");
    audio_event_iface_cfg_t evt_cfg = AUDIO_EVENT_IFACE_DEFAULT_CFG();
    evt = audio_event_iface_init(&evt_cfg);
    audio_pipeline_set_listener(pipeline, evt);
    audio_event_iface_set_listener(esp_periph_set_get_event_iface(set), evt);

    audio_pipeline_run(pipeline);
    AudioPlaying = true;


    // Ensure BT never disconnects
    xTaskCreatePinnedToCore(
        bluetooth_monitor_task,
        "BT_Monitor",
        4096,
        NULL,
        5,
        &xBtTaskHandler,
        1);

    ESP_LOGI(TAG, "bluetooth_monitor_task created");

}

void bluetooth_monitor_task(void *param)
{
    AudioPlaying = true;
    while (1) {
        audio_event_iface_msg_t msg;
        esp_err_t ret = audio_event_iface_listen(evt, &msg, portMAX_DELAY);
        if (ret == ESP_OK && msg.source_type == PERIPH_ID_BLUETOOTH && msg.source == (void *)bt_periph) {
            if (msg.cmd == PERIPH_BLUETOOTH_DISCONNECTED || msg.cmd == PERIPH_BLUETOOTH_AUDIO_SUSPENDED) {
                ESP_LOGW(TAG, "[ * ] Bluetooth disconnected or suspended, attempting reconnect");
                periph_bt_stop(bt_periph);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                // periph_bt_start(bt_periph);
                esp_periph_start(set, bt_periph);
            }
        }
    }
}


bool isAudioStarted()
{
    return AudioPlaying;
}

void A2dpAudioStop()
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
    
    audio_pipeline_remove_listener(pipeline);
    esp_periph_set_stop_all(set);
    audio_event_iface_remove_listener(esp_periph_set_get_event_iface(set), evt);
    
    audio_event_iface_destroy(evt);
    audio_pipeline_unregister(pipeline, bt_stream_writer);
    audio_pipeline_unregister(pipeline, fatfs_stream_reader);
    audio_pipeline_unregister(pipeline, music_decoder);
    audio_pipeline_deinit(pipeline);
    audio_element_deinit(bt_stream_writer);
    audio_element_deinit(fatfs_stream_reader);
    audio_element_deinit(music_decoder);

    esp_periph_set_destroy(set); // Move this before Bluetooth cleanup

    esp_bluedroid_disable();
    esp_bluedroid_deinit();
    esp_bt_controller_disable();
    esp_bt_controller_deinit();
    esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
}


// void startA2dpService()
// {
//     audio_pipeline_handle_t pipeline;
//     audio_element_handle_t fatfs_stream_reader, bt_stream_writer, music_decoder;

//     // esp_log_level_set("*", ESP_LOG_ERROR);
//     // esp_log_level_set(TAG, ESP_LOG_ERROR);

//     esp_err_t err = nvs_flash_init();
//     if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
//         // NVS partition was truncated and needs to be erased
//         // Retry nvs_flash_init
//         ESP_ERROR_CHECK(nvs_flash_erase());
//         err = nvs_flash_init();
//     }

//     ESP_LOGI(TAG, "[ 1 ] Mount sdcard");
//     // Initialize peripherals management
//     esp_periph_config_t periph_cfg = DEFAULT_ESP_PERIPH_SET_CONFIG();
//     esp_periph_set_handle_t set = esp_periph_set_init(&periph_cfg);

//     // Initialize SD Card peripheral
//     audio_board_sdcard_init(set, SD_MODE_SPI);

//     ESP_LOGI(TAG, "[3.0] Create audio pipeline for playback");
//     audio_pipeline_cfg_t pipeline_cfg = DEFAULT_AUDIO_PIPELINE_CONFIG();
//     pipeline = audio_pipeline_init(&pipeline_cfg);
//     mem_assert(pipeline);

//     ESP_LOGI(TAG, "[3.1] Create fatfs stream to read data from sdcard");
//     fatfs_stream_cfg_t fatfs_cfg = FATFS_STREAM_CFG_DEFAULT();
//     fatfs_cfg.type = AUDIO_STREAM_READER;
//     fatfs_stream_reader = fatfs_stream_init(&fatfs_cfg);

//     // list_sdcard_files(MOUNT_POINT); // list files 

//     ESP_LOGI(TAG, "[3.2] Create mp3/wav decoder to decode mp3/wav file");
    
//     if (strstr(file_path, ".mp3")) {
//         mp3_decoder_cfg_t mp3_cfg = DEFAULT_MP3_DECODER_CONFIG();
//         music_decoder = mp3_decoder_init(&mp3_cfg);
//     } else if (strstr(file_path, ".wav")) {
//         wav_decoder_cfg_t wav_cfg = DEFAULT_WAV_DECODER_CONFIG();
//         music_decoder = wav_decoder_init(&wav_cfg);
//     } else {
//         ESP_LOGE(TAG, "Unsupported file format!");
//         return;
//     }
//     mem_assert(music_decoder);

//     ESP_LOGI(TAG, "[3.3] Create Bluetooth stream");

//     esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
//     esp_bt_pin_type_t pin_type = ESP_BT_PIN_TYPE_FIXED;
//     esp_bt_pin_code_t pin_code = {'1', '2', '3', '4'};
//     ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));
//     ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
//     ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT));
//     ESP_ERROR_CHECK(esp_bluedroid_init());
//     ESP_ERROR_CHECK(esp_bluedroid_enable());

//     esp_bt_dev_set_device_name("ESP_SOURCE_STREAM_DEMO");
//     esp_bt_gap_set_pin(pin_type, 4, pin_code);
//     esp_bt_gap_register_callback(bt_app_gap_cb);

//     const char *remote_name = NULL;
//     remote_name = "RONIN (R-140)"; // CONFIG_BT_REMOTE_NAME;
//     if (remote_name) {
//         memcpy(&remote_bt_device_name, remote_name, strlen(remote_name) + 1);
//     } else {
//         memcpy(&remote_bt_device_name, "ESP_SINK_STREAM_DEMO", ESP_BT_GAP_MAX_BDNAME_LEN);
//     }

//     a2dp_stream_config_t a2dp_config = {
//         .type = AUDIO_STREAM_WRITER,
//         .user_callback = {0},
//     };
//     bt_stream_writer = a2dp_stream_init(&a2dp_config);

//     esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);
// #if (ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 0, 0))
//     esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
// #else
//     esp_bt_gap_set_scan_mode(ESP_BT_SCAN_MODE_CONNECTABLE_DISCOVERABLE);
// #endif


//     ESP_LOGI(TAG, "[3.4] Register all elements to audio pipeline");
//     audio_pipeline_register(pipeline, fatfs_stream_reader, "file");
//     audio_pipeline_register(pipeline, music_decoder, "dec");
//     audio_pipeline_register(pipeline, bt_stream_writer, "bt");

//     ESP_LOGI(TAG, "[3.5] Link it together [sdcard]-->fatfs_stream-->music_decoder-->bt_stream-->[bt sink]");
//     const char *link_tag[3] = {"file", "dec", "bt"};
//     audio_pipeline_link(pipeline, &link_tag[0], 3);

//     ESP_LOGI(TAG, "[3.6] Set up  uri (file as fatfs_stream, file as mp3/wav decoder, and default output is i2s)");
//     audio_element_set_uri(fatfs_stream_reader, file_path);

//     ESP_LOGI(TAG, "[3.7] Create bt peripheral");
//     esp_periph_handle_t bt_periph = bt_create_periph();

//     ESP_LOGI(TAG, "[3.8] Start bt peripheral");
//     esp_periph_start(set, bt_periph);

//     ESP_LOGI(TAG, "[ 4 ] Set up  event listener");
//     audio_event_iface_cfg_t evt_cfg = AUDIO_EVENT_IFACE_DEFAULT_CFG();
//     audio_event_iface_handle_t evt = audio_event_iface_init(&evt_cfg);

//     ESP_LOGI(TAG, "[4.1] Listening event from all elements of pipeline");
//     audio_pipeline_set_listener(pipeline, evt);

//     ESP_LOGI(TAG, "[4.2] Listening event from peripherals");
//     audio_event_iface_set_listener(esp_periph_set_get_event_iface(set), evt);

//     ESP_LOGI(TAG, "[ 5 ] Start audio_pipeline");
//     audio_pipeline_run(pipeline);

//     ESP_LOGI(TAG, "[ 6 ] Listen for all pipeline events");
//     while (1) {
//         audio_event_iface_msg_t msg;
//         esp_err_t ret = audio_event_iface_listen(evt, &msg, portMAX_DELAY);
//         if (ret != ESP_OK) {
//             ESP_LOGE(TAG, "[ * ] Event interface error : %d", ret);
//             continue;
//         }

//         if (msg.source_type == AUDIO_ELEMENT_TYPE_ELEMENT
//             && msg.source == (void *) music_decoder
//             && msg.cmd == AEL_MSG_CMD_REPORT_MUSIC_INFO) {
//             audio_element_info_t music_info = {0};
//             audio_element_getinfo(music_decoder, &music_info);

//             ESP_LOGI(TAG,"[ * ] Receive music info from music decoder, sample_rates=%d, bits=%d, ch=%d",
//                     music_info.sample_rates, music_info.bits, music_info.channels);
//             continue;
//         }
//         if (msg.source_type == PERIPH_ID_BLUETOOTH
//             && msg.source == (void *)bt_periph) {
//             if ((msg.cmd == PERIPH_BLUETOOTH_DISCONNECTED) || (msg.cmd == PERIPH_BLUETOOTH_AUDIO_SUSPENDED)) {
//                 ESP_LOGW(TAG, "[ * ] Bluetooth disconnected or suspended");
//                 periph_bt_stop(bt_periph);
//                 break;
//             }
//         }
//     }

//     ESP_LOGI(TAG, "[ 7 ] Stop audio_pipeline");
//     audio_pipeline_stop(pipeline);
//     audio_pipeline_wait_for_stop(pipeline);
//     audio_pipeline_terminate(pipeline);

//     /* Terminal the pipeline before removing the listener */
//     audio_pipeline_remove_listener(pipeline);

//     /* Stop all periph before removing the listener */
//     esp_periph_set_stop_all(set);
//     audio_event_iface_remove_listener(esp_periph_set_get_event_iface(set), evt);

//     /* Make sure audio_pipeline_remove_listener & audio_event_iface_remove_listener are called before destroying event_iface */
//     audio_event_iface_destroy(evt);

//     /* Release all resources */
//     audio_pipeline_unregister(pipeline, bt_stream_writer);
//     audio_pipeline_unregister(pipeline, fatfs_stream_reader);
//     audio_pipeline_unregister(pipeline, music_decoder);
//     audio_pipeline_deinit(pipeline);
//     audio_element_deinit(bt_stream_writer);
//     audio_element_deinit(fatfs_stream_reader);
//     audio_element_deinit(music_decoder);
//     esp_periph_set_destroy(set);
//     esp_bluedroid_disable();
//     esp_bluedroid_deinit();
//     esp_bt_controller_disable();
//     esp_bt_controller_deinit();
//     esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
// }
