/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/integration/framework/arduino.html  */

#include <lvgl.h>
#include "src/screens/screens.h"

#if LV_USE_TFT_ESPI
#include <TFT_eSPI.h>
#endif


#include <demos/lv_demos.h>

/*Set to your screen resolution and rotation*/
#define TFT_HOR_RES   240
#define TFT_VER_RES   240
#define TFT_ROTATION  LV_DISPLAY_ROTATION_270
#define DRAW_BUF_SIZE (TFT_HOR_RES * 40)
// #define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_HOR_RES * 2)

#if LV_USE_LOG != 0
void my_print( lv_log_level_t level, const char * buf )
{
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif


void memLog();

/*use Arduinos millis() as tick source*/
static uint32_t my_tick(void)
{
    return millis();
}

void setup()
{
    Serial.begin( 115200 );

    lv_init();

    /*Set a tick source so that LVGL will know how much time elapsed. */
    lv_tick_set_cb(my_tick);

    static void *buf0 = (lv_color_t *)heap_caps_malloc(DRAW_BUF_SIZE, MALLOC_CAP_SPIRAM);
    static void *buf1 = (lv_color_t *)heap_caps_malloc(DRAW_BUF_SIZE, MALLOC_CAP_SPIRAM);

    // lv_color_t *buf1 = (lv_color_t *)malloc(TFT_HOR_RES * TFT_VER_RES * sizeof(lv_color_t));
    // buf1 = heap_caps_malloc(TFT_HOR_RES * 100 * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
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
    disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, buf0, buf1, size_in_px, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_rotation(disp, TFT_ROTATION);

#else
    /*Else create a display yourself*/
    disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, buf0, buf1, size_in_px, LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif

   
    // lv_obj_clean(lv_screen_active());
    // lv_demo_widgets();

    // lv_obj_clean(lv_screen_active());
    // // lv_obj_set_style_bg_color(lv_screen_active(), lv_color_black(), LV_PART_MAIN);
    // // lv_obj_t *label = lv_label_create( lv_screen_active() );
    // // lv_label_set_text( label, "Hello Arduino, I'm 0000!" );
    // // lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

    // // delay(5000); /* let this time pass */
    // // lv_obj_clean(lv_screen_active());
    // // lv_demo_stress();
     
    // // lv_obj_set_style_bg_color(lv_screen_active(), lv_color_black(), LV_PART_MAIN);
    memLog();

    // lv_disp_t *dispp = lv_display_get_default();
    lv_theme_t *theme = lv_theme_default_init(disp, lv_color_white(), lv_color_black(),
                                              true, LV_FONT_DEFAULT);

    lv_disp_set_theme(disp, theme);

    // Initialize screen and set background
    // lv_obj_clean(lv_screen_active());
    // ui_EndGong1_screen_init();
    // ui_MeditationScreen_screen_init();
    // ui_BluetoothScreen_screen_init();
    // ui_SetTimerScr_screen_init();
    ui_SoundScreen_screen_init();

    // lv_disp_load_scr(ui_EndGong1);
    // lv_disp_load_scr(ui_BluetoothScreen);
    // lv_disp_load_scr(ui_SetTimerScr);
    lv_disp_load_scr(ui_SoundScreen);

    memLog();
}

void loop()
{
    lv_timer_handler(); /* let the GUI do its work */
    delay(5); /* let this time pass */

    if(millis()%5000 == 0)
    {
      memLog();
    }    
}

void memLog()
{
  Serial.printf("Free heap: %d\n", esp_get_free_heap_size());
  Serial.printf("Largest block: %d\n", heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));
  Serial.printf(
  "  MALLOC_CAP_8BIT      %7zu bytes\n"
  "  MALLOC_CAP_DMA       %7zu bytes\n"
  "  MALLOC_CAP_SPIRAM    %7zu bytes\n"
  "  MALLOC_CAP_INTERNAL  %7zu bytes\n"
  "  MALLOC_CAP_DEFAULT   %7zu bytes\n"
  "  MALLOC_CAP_IRAM_8BIT %7zu bytes\n"
  "  MALLOC_CAP_RETENTION %7zu bytes\n",
  heap_caps_get_free_size(MALLOC_CAP_8BIT),
  heap_caps_get_free_size(MALLOC_CAP_DMA),
  heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
  heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
  heap_caps_get_free_size(MALLOC_CAP_DEFAULT),
  heap_caps_get_free_size(MALLOC_CAP_IRAM_8BIT),
  heap_caps_get_free_size(MALLOC_CAP_RETENTION));
}
