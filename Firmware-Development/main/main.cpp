/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "SystemInit.h"
#include "lvgl.h"


#if LV_USE_TFT_ESPI
#include <TFT_eSPI.h>
#endif


/*Set to your screen resolution and rotation*/
static const char *TAG = "MAIN";
#define TFT_HOR_RES   360
#define TFT_VER_RES   360
#define TFT_ROTATION  LV_DISPLAY_ROTATION_270
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_HOR_RES)
void lvglTask(void *pvParameters);

#if LV_USE_LOG != 0
void my_print( lv_log_level_t level, const char * buf )
{
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

static uint32_t my_tick(void)
{
    return xTaskGetTickCount();
}

extern "C" void app_main(void)
{
    btAudio.mountSDcard();
    xTaskCreatePinnedToCore( lvglTask,"LVGL Task", 8192, NULL, 5, NULL, 0);
    
    // Read a favourite from JSON
    Favourite loadedFav;
    storagCard.read_favourite_from_json("Favourite 1", &loadedFav);
    storagCard.read_favourite_from_json("Favourite 2", &loadedFav);
    storagCard.read_favourite_from_json("Favourite 3", &loadedFav);
    storagCard.read_favourite_from_json("Favourite 4", &loadedFav);
    storagCard.read_favourite_from_json("Favourite 5", &loadedFav);
    storagCard.read_favourite_from_json("Favourite 6", &loadedFav);

    return;
}



void lvglTask(void *pvParameters)
{  
    // Turn on the back-light LED & set default brightness 50%
    Core.setupPWM();

    lv_init();
    /*Set a tick source so that LVGL will know how much time elapsed. */
    lv_tick_set_cb(my_tick);
    static void *buf0 = (lv_color_t *)heap_caps_malloc(DRAW_BUF_SIZE, MALLOC_CAP_SPIRAM);
    static void *buf1 = (lv_color_t *)heap_caps_malloc(DRAW_BUF_SIZE, MALLOC_CAP_SPIRAM);

    assert(buf0 != NULL);
    assert(buf1 != NULL);
    uint32_t size_in_px = DRAW_BUF_SIZE;
    /* register print function for debugging */
#if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print );
#endif

    lv_display_t * disp;
#if LV_USE_TFT_ESPI
    /*TFT_eSPI can be enabled lv_conf.h to initialize the display in a simple way*/
    disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, buf0, NULL, size_in_px, LV_DISPLAY_RENDER_MODE_PARTIAL);
    // lv_display_set_rotation(disp, TFT_ROTATION);

#else
    /*Else create a display yourself*/
    disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, buf0, NULL, size_in_px, LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif

    /*Initialize the Input device driver*/
    lv_indev_t * indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_KEYPAD);
    lv_indev_set_read_cb(indev, System::ButtonReadStatic);
    
    
    // lv_obj_clean(lv_screen_active());
    lv_theme_t *theme = lv_theme_default_init(disp, lv_color_white(), lv_color_black(),
    true, LV_FONT_DEFAULT);
    
    lv_disp_set_theme(disp, theme);
    
    // Initialize screen and set background
    Core.Initialize();
    lv_disp_load_scr(Screen[0]);
    memLog();

    while (1)
    {
        /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
        lv_timer_handler(); /* let the GUI do its work */
        vTaskDelay(pdMS_TO_TICKS(3));
    }
}







 