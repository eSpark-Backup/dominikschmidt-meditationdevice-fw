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

typedef struct {
    int timer;
    char soundType[50];
    char soundNumber[50];
    bool endGong;
} Favourite;

class SdCard
{
    public:
        void mountSDcard();
        void list_sdcard_files(const char *path);
        void save_favourite_to_json(const char* fav_name, Favourite fav);
        bool read_favourite_from_json(const char* fav_name, Favourite* fav);
    private:
        bool check_sd_card();

};

extern SdCard storagCard;

#endif /*SDCARD_HPP */