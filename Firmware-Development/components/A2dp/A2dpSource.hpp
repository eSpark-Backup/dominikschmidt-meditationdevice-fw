#ifndef A2DPSOURCE_H
#define A2DPSOURCE_H

#include <string.h>
#include <inttypes.h>
#include "esp_log.h"
#include "esp_wifi.h"
#include "sdkconfig.h"
#include "esp_peripherals.h"
#include "esp_gap_bt_api.h"

#include "nvs_flash.h"
#include "audio_element.h"
#include "audio_pipeline.h"
#include "audio_event_iface.h"
#include "board.h"
#include "mp3_decoder.h"
#include "wav_decoder.h"
#include "a2dp_stream.h"
#include "fatfs_stream.h"
#include <dirent.h>

void startA2dpService(void);


#endif /*A2DPSOURCE_H */