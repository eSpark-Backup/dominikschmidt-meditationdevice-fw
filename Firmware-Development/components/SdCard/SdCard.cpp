#include "SdCard.hpp"
#include "A2dpSource.hpp"

#define FAV_MOUNT_POINT "/sdcard"
#define AUDIO_PATH "/sdcard/audio"
static const char *TAG = "Storage";
Storage storagCard;

void Storage::mountSDcard()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    
    ESP_LOGI(TAG, "[ 1 ] Mount sdcard");
    esp_periph_config_t periph_cfg = DEFAULT_ESP_PERIPH_SET_CONFIG();
    esp_periph_set_handle_t setSD = esp_periph_set_init(&periph_cfg);
    audio_board_sdcard_init(setSD, SD_MODE_SPI);

    btAudio.setSdcardPeriph(setSD);
    // list_sdcard_files(AUDIO_PATH);
}

void Storage::list_sdcard_files(const char *path)
{
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

void Storage::loadFolderToDropdown(const char *folderName, lv_obj_t *dropdown)
{
    char path[256];
    snprintf(path, sizeof(path), "%s/%s", AUDIO_PATH, folderName);

    DIR *dir = opendir(path);
    if (!dir) return;

    struct dirent *entry;
    char *filenames[256];
    int count = 0;
    size_t total_length = 1;

    while ((entry = readdir(dir)) != NULL && count < 256) {
        if (entry->d_type == DT_REG) {
            size_t len = strlen(entry->d_name);
            filenames[count] = (char *)malloc(len + 1);
            if (filenames[count]) {
                strcpy(filenames[count], entry->d_name);
                total_length += len + 1;
                count++;
            }
        }
    }

    char *option_str = (char *)malloc(total_length);
    if (!option_str) {
        for (int i = 0; i < count; i++) free(filenames[i]);
        closedir(dir);
        return;
    }

    option_str[0] = '\0';
    for (int i = 0; i < count; i++) {
        strcat(option_str, filenames[i]);
        if (i < count - 1) strcat(option_str, "\n");
        free(filenames[i]);
    }
    lv_dropdown_set_options(dropdown, option_str);
    free(option_str);
    closedir(dir);
}

void Storage::updateSoundListFromSdCard()
{
    loadFolderToDropdown("Nature", ui_NatureSoundDropList);
    loadFolderToDropdown("BinauralBeats", ui_BinauralBeatDropList);
    loadFolderToDropdown("WhiteNoise", ui_WhiteNoiseDropList);
    loadFolderToDropdown("Lessons", ui_LessonsDropList);
}

esp_err_t Storage::saveFavourite(const char* fav_name, const Favourite &fav)
{
    nvs_handle_t handle;
    esp_err_t ret = nvs_open("favorites", NVS_READWRITE, &handle);
    if (ret == ESP_OK) {
        ret = nvs_set_blob(handle, fav_name, &fav, sizeof(Favourite));
        if (ret == ESP_OK) {
            ret = nvs_commit(handle);
        }
        nvs_close(handle);
    }

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to save favourite '%s': %s", fav_name, esp_err_to_name(ret));
    }
    
    return ret;
}

esp_err_t Storage::readFavourite(const char* fav_name, Favourite &fav)
{
    nvs_handle_t handle;
    size_t required_size = sizeof(Favourite);
    esp_err_t ret = nvs_open("favorites", NVS_READONLY, &handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open NVS namespace: %s", esp_err_to_name(ret));
        return ret;
    }
    
    ret = nvs_get_blob(handle, fav_name, &fav, &required_size);
    nvs_close(handle);
    
    if (ret == ESP_OK) {
        if (required_size != sizeof(Favourite)) {
            ESP_LOGW(TAG, "Size mismatch when reading favourite '%s' (expected %d, got %d)",
                fav_name, (int)sizeof(Favourite), (int)required_size);
                ret = ESP_ERR_NVS_INVALID_LENGTH;
            }
        } else {
            ESP_LOGE(TAG, "Failed to read favourite '%s': %s", fav_name, esp_err_to_name(ret));
        }
        
        return ret;
}
    
esp_err_t Storage::ensureNvsInitialized()
{
    nvs_handle_t handle;
    esp_err_t ret = nvs_open("favorites", NVS_READWRITE, &handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open NVS namespace: %s", esp_err_to_name(ret));
        return ret;
    }

    uint8_t init_marker = 0;
    ret = nvs_get_u8(handle, "init_done", &init_marker);

    if (ret == ESP_ERR_NVS_NOT_FOUND) {
        // First time init — write marker
        esp_err_t write_ret = nvs_set_u8(handle, "init_done", 1);
        if (write_ret == ESP_OK) {
            write_ret = nvs_commit(handle);
        }

        if (write_ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to write init marker: %s", esp_err_to_name(write_ret));
        } else {
            ESP_LOGI(TAG, "NVS initialized for the first time.");
        }

        nvs_close(handle);
        return write_ret;
    } else if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read init marker: %s", esp_err_to_name(ret));
    }

    nvs_close(handle);
    return ret;
}

bool Storage::check_sd_card()
{
    #define MOUNT_POINT "/sdcard"

    FILE *f = fopen(MOUNT_POINT "/test.txt", "w");
    if (f) {
        fclose(f);
        remove(MOUNT_POINT "/test.txt"); // Delete test file
        ESP_LOGI(TAG, "SD card is accessible.");
        return true;
    } else {
        ESP_LOGE(TAG, "SD card not accessible!");
        return false;
    }

    // esp_vfs_fat_sdmmc_mount_config_t mount_config = {
    //     .format_if_mount_failed = false,
    //     .max_files = 5
    // };

    // sdmmc_card_t* card;
    // esp_err_t ret = esp_vfs_fat_sdmmc_mount(MOUNT_POINT, &mount_config, &card);

    // if (ret == ESP_OK) {
    //     ESP_LOGI(TAG, "SD card mounted successfully.");
    //     return true;  // SD card is connected
    // } else {
    //     ESP_LOGE(TAG, "Failed to mount SD card! Error: %s", esp_err_to_name(ret));
    //     return false; // SD card is not connected
    // }
}