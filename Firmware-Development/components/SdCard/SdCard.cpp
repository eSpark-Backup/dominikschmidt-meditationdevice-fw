#include "SdCard.hpp"
#include "A2dpSource.hpp"

#define FAV_MOUNT_POINT "/sdcard"
#define AUDIO_PATH "/sdcard/audio"
static const char *TAG = "SdCard";
SdCard storagCard;

void SdCard::mountSDcard()
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

void SdCard::list_sdcard_files(const char *path)
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

void SdCard::save_favourite_to_json(const char* fav_name, Favourite fav)
{
    if (check_sd_card()) {
        ESP_LOGI(TAG, "SD Card is present.");
    } else {
        ESP_LOGE(TAG, "SD Card is not detected.");
        return;
    }

    cJSON* json;
    FILE* f = fopen(FAV_MOUNT_POINT"/sdcard/favourites.json", "r");
    if (!f) {
        // File does not exist, so create it
        f = fopen(FAV_MOUNT_POINT"/sdcard/favourites.json", "w");
        if (!f) {
            ESP_LOGE(TAG, "Failed to create JSON file!");
            return;
        }
        fprintf(f, "{}"); // Initialize with an empty JSON object
        fclose(f);
    
        json = cJSON_CreateObject();  // Now create a new JSON object in memory
    } else {
        // Read existing file
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        char* buffer = (char*)malloc(fsize + 1);
        if (!buffer) {
            fclose(f);
            ESP_LOGE(TAG, "Memory allocation failed!");
            return;
        }
        size_t bytesRead = fread(buffer, 1, fsize, f);
        if (bytesRead != fsize) {
            fclose(f);
            free(buffer);
            ESP_LOGE(TAG, "Failed to read JSON file completely!");
            return;
        }

        buffer[fsize] = '\0';
        fclose(f);

        json = cJSON_Parse(buffer);
        free(buffer);
        if (!json) {
            json = cJSON_CreateObject();  // Create new JSON if parsing fails
        }
    }

    // Create a JSON object for the new favourite
    cJSON* favJson = cJSON_CreateObject();
    cJSON_AddNumberToObject(favJson, "Timer", fav.timer);
    cJSON_AddStringToObject(favJson, "SoundType", fav.soundType);
    cJSON_AddStringToObject(favJson, "SoundNumber", fav.soundNumber);
    cJSON_AddBoolToObject(favJson, "EndGong", fav.endGong);

    // Add to the main JSON structure
    if (cJSON_HasObjectItem(json, fav_name)) {
        cJSON_ReplaceItemInObject(json, fav_name, favJson);
    } else {
        cJSON_AddItemToObject(json, fav_name, favJson);
    }
    

    // Write back to file
    f = fopen(FAV_MOUNT_POINT"/sdcard/favourites.json", "w");
    if (f) {
        char* jsonStr = cJSON_Print(json);
        fprintf(f, "%s", jsonStr);
        fclose(f);
        free(jsonStr);
        ESP_LOGI(TAG, "Saved %s to JSON", fav_name);
    } else {
        ESP_LOGE(TAG, "Failed to write JSON file!");
    }

    cJSON_Delete(json);
}

bool SdCard::read_favourite_from_json(const char* fav_name, Favourite* fav)
{
    if (check_sd_card()) {
        ESP_LOGI(TAG, "SD Card is present.");
    } else {
        ESP_LOGE(TAG, "SD Card is not detected.");
        return false;
    }

    
    FILE* f = fopen(FAV_MOUNT_POINT"/sdcard/favourites.json", "r");
    if (!f) {
        ESP_LOGE(TAG, "JSON file not found!");
        return false;
    }

    // Read the file
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* buffer = (char*)malloc(fsize + 1);
    if (!buffer) {
        fclose(f);
        ESP_LOGE(TAG, "Memory allocation failed!");
        return false;
    }
    size_t bytesRead = fread(buffer, 1, fsize, f);
    if (bytesRead != fsize) {
        fclose(f);
        free(buffer);
        ESP_LOGE(TAG, "Failed to read JSON file completely!");
        return false;
    }
    buffer[fsize] = '\0';
    fclose(f);

    // Parse JSON
    cJSON* json = cJSON_Parse(buffer);
    free(buffer);
    if (!json) {
        ESP_LOGE(TAG, "JSON parsing failed!");
        return false;
    }

    // Get the requested favourite
    cJSON* favJson = cJSON_GetObjectItem(json, fav_name);
    if (!favJson) {
        ESP_LOGE(TAG, "%s not found in JSON!", fav_name);
        cJSON_Delete(json);
        return false;
    }

    fav->timer = cJSON_GetObjectItem(favJson, "Timer")->valueint;
    strncpy(fav->soundType, cJSON_GetObjectItem(favJson, "SoundType")->valuestring, sizeof(fav->soundType) - 1);
    fav->soundType[sizeof(fav->soundType) - 1] = '\0'; // Ensure null termination

    strncpy(fav->soundNumber, cJSON_GetObjectItem(favJson, "SoundNumber")->valuestring, sizeof(fav->soundNumber) - 1);
    fav->soundNumber[sizeof(fav->soundNumber) - 1] = '\0'; // Ensure null termination

    fav->endGong = cJSON_IsTrue(cJSON_GetObjectItem(favJson, "EndGong"));

    ESP_LOGI(TAG, "Loaded %s: Timer=%d, SoundType=%s, SoundNumber=%s, EndGong=%s",
             fav_name, fav->timer, fav->soundType, fav->soundNumber,
             fav->endGong ? "True" : "False");

    cJSON_Delete(json);
    return true;
}

bool SdCard::check_sd_card()
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