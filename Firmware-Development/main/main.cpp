/* Meditation Device [DOMINIK] LVGL - A2DP

   This example code is in the private Domain (or CC0 licensed, at your option.)

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
#include <TFT_eSPI.h>

/*Set to your screen resolution and rotation*/
static const char *TAG = "MAIN";
#define TFT_HOR_RES   360
#define TFT_VER_RES   360
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_HOR_RES)


void lvglTask(void *pvParameters);
static uint32_t my_tick(void);

extern "C" void app_main(void)
{
    xTaskCreatePinnedToCore(lvglTask, "LVGL Task", 8192, NULL, 5, NULL, 1);

    storagCard.mountSDcard();  // This may take time
    // Favourite loadedFav;
    // for (int i = 1; i <= 6; i++) {
    //     char name[16];
    //     snprintf(name, sizeof(name), "Favourite %d", i);
    //     storagCard.read_favourite_from_json(name, &loadedFav);
    // }
    return;
}

void lvglTask(void *pvParameters)
{  
    lv_init();
    /*Set a tick source so that LVGL will know how much time elapsed. */
    lv_tick_set_cb(my_tick);
    static void *buf0 = (lv_color_t *)heap_caps_malloc(DRAW_BUF_SIZE, MALLOC_CAP_SPIRAM);
    assert(buf0 != NULL);
    uint32_t size_in_px = DRAW_BUF_SIZE;    
    lv_display_t * disp;
    disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, buf0, NULL, size_in_px, LV_DISPLAY_RENDER_MODE_PARTIAL);

    // Initialize screen and set background with 50% brightness
    Core.InitializeScreen();
    Core.setupPWM();
    lv_disp_load_scr(ui_SetTimerScreen);
    lv_timer_handler(); /* let the GUI do its work */

    /*Initialize the Input device driver*/
    lv_indev_t * indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_KEYPAD);
    lv_indev_set_read_cb(indev, System::ButtonReadStatic);

    lv_theme_t *theme = lv_theme_default_init(disp, lv_color_white(), lv_color_black(), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, theme);
   
    // Init everything else
    Core.Initialize();
    memLog();
    
    while (1)
    {
        /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
        lv_timer_handler(); /* let the GUI do its work */
        Core.updateBluetoothSettings();
        vTaskDelay(pdMS_TO_TICKS(3));
    }
}

static uint32_t my_tick(void)
{
    return xTaskGetTickCount();
}
 