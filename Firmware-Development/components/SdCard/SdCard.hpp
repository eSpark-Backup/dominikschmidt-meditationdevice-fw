#ifndef SDCARD_HPP
#define SDCARD_HPP

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "esp_log.h"
#include "sdkconfig.h"
#include "cJSON.h"
#include "esp_peripherals.h"
#include "nvs_flash.h"
#include "lvgl.h"
#include "A2dpSource.hpp"

typedef struct {
    int timer;
    char soundType[50];
    char soundNumber[50];
    bool endGong;
} Favourite;

class Storage
{
    public:
        void mountSDcard();
        void list_sdcard_files(const char *path);
        void updateSoundListFromSdCard();
        esp_err_t ensureNvsInitialized();
        esp_err_t saveBtDevice(ScannedDevice* btDevice);
        esp_err_t readBtDevice(ScannedDevice* dev);
        esp_err_t saveFavourite(const char* fav_name, const Favourite &fav);
        esp_err_t readFavourite(const char* fav_name, Favourite &fav);
        char* getBtDevice();
    private:
        bool check_sd_card();
        void loadFolderToDropdown(const char *folderName, lv_obj_t *dropdown);
};

extern Storage storagCard;

#endif /*SDCARD_HPP */