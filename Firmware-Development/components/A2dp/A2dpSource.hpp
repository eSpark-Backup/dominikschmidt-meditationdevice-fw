#ifndef A2DPSOURCE_H
#define A2DPSOURCE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "esp_log.h"
#include "esp_wifi.h"
#include "sdkconfig.h"
#include "esp_bt_device.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"

#include "audio_element.h"
#include "audio_pipeline.h"
#include "audio_event_iface.h"
#include "board.h"
#include "mp3_decoder.h"
#include "wav_decoder.h"
#include "a2dp_stream.h"
#include "fatfs_stream.h"
#include <dirent.h>
#include "screens.h"



class A2DP
{
    //Variables
    private:
        esp_periph_set_handle_t set;

    //Methods
    public:
        void A2dpServiceInit();
        void A2dpAudioStart(char *filepath, int play_time_sec = 0);
        void A2dpAudioStop();
        bool isAudioStarted();
        void update_dropdown_options(lv_obj_t *dropdown, char **options, int option_count);
        void start_bt_scan();
        bool connect_to_bt_device(const char *device_name);
        bool disconnect_to_bt_device(const char *device_name);
        void print_bonded_devices();
        void setSdcardPeriph(esp_periph_set_handle_t sd);
        
    private:
        static void loopAudioTask(void *param);
        static void bt_app_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param);
        static void filter_inquiry_scan_result(esp_bt_gap_cb_param_t *param);
        static bool get_name_from_eir(uint8_t *eir, uint8_t *bdname, uint8_t *bdname_len);
        static char *bda2str(esp_bd_addr_t bda, char *str, size_t size);
        static void update_bt_dropdown(lv_obj_t *dropdown);

};

// Structure to pass data to the task
typedef struct {
    A2DP* instance;    // Pointer to A2DP instance
    int64_t start_time;
    int64_t play_time_us;
} AudioTaskArgs;

extern A2DP btAudio;
extern bool deviceConnected;
extern int audioCount;
#endif /*A2DPSOURCE_H */