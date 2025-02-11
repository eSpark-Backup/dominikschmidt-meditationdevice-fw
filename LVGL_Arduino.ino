/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/integration/framework/arduino.html  */

#include <lvgl.h>
#include "ui_EndGong.h"

#if LV_USE_TFT_ESPI
#include <TFT_eSPI.h>
#endif

/*To use the built-in examples and demos of LVGL uncomment the includes below respectively.
 *You also need to copy `lvgl/examples` to `lvgl/src/examples`. Similarly for the demos `lvgl/demos` to `lvgl/src/demos`.
 *Note that the `lv_examples` library is for LVGL v7 and you shouldn't install it for this version (since LVGL v8)
 *as the examples and demos are now part of the main LVGL library. */

// #include <examples/lv_examples.h>
#include <demos/lv_demos.h>

/*Set to your screen resolution and rotation*/
#define TFT_HOR_RES   240
#define TFT_VER_RES   240
#define TFT_ROTATION  LV_DISPLAY_ROTATION_270

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
// #define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
// uint32_t draw_buf[DRAW_BUF_SIZE / 4];
#define DRAW_BUF_SIZE (TFT_HOR_RES *40)

#if LV_USE_LOG != 0
void my_print( lv_log_level_t level, const char * buf )
{
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

/* LVGL calls it when a rendered image needs to copied to the display*/
void my_disp_flush( lv_display_t *disp, const lv_area_t *area, uint8_t * px_map)
{
    /*Copy `px map` to the `area`*/

    /*For example ("my_..." functions needs to be implemented by you)
    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);

    my_set_window(area->x1, area->y1, w, h);
    my_draw_bitmaps(px_map, w * h);
     */

    /*Call it to tell LVGL you are ready*/
    lv_display_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read( lv_indev_t * indev, lv_indev_data_t * data )
{
    /*For example  ("my_..." functions needs to be implemented by you)
    int32_t x, y;
    bool touched = my_get_touch( &x, &y );

    if(!touched) {
        data->state = LV_INDEV_STATE_RELEASED;
    } else {
        data->state = LV_INDEV_STATE_PRESSED;

        data->point.x = x;
        data->point.y = y;
    }
     */
}

/*use Arduinos millis() as tick source*/
static uint32_t my_tick(void)
{
    return millis();
}

void setup()
{
    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.begin( 115200 );
    Serial.println( LVGL_Arduino );

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
    disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, buf0, size_in_px);
    lv_display_set_rotation(disp, TFT_ROTATION);

#else
    /*Else create a display yourself*/
    disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, buf1, buf0, size_in_px, LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif

    // /*Initialize the (dummy) input device driver*/
    // lv_indev_t * indev = lv_indev_create();
    // lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touchpad should have POINTER type*/
    // lv_indev_set_read_cb(indev, my_touchpad_read);

    /* Create a simple label
     * ---------------------
     lv_obj_t *label = lv_label_create( lv_screen_active() );
     lv_label_set_text( label, "Hello Arduino, I'm LVGL!" );
     lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

     * Try an example. See all the examples
     *  - Online: https://docs.lvgl.io/master/examples.html
     *  - Source codes: https://github.com/lvgl/lvgl/tree/master/examples
     * ----------------------------------------------------------------

     lv_example_btn_1();

     * Or try out a demo. Don't forget to enable the demos in lv_conf.h. E.g. LV_USE_DEMO_WIDGETS
     * -------------------------------------------------------------------------------------------

     lv_demo_widgets();
     */
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

    lv_disp_t *dispp = lv_display_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_color_white(), lv_color_black(),
                                              true, LV_FONT_DEFAULT);

    lv_disp_set_theme(dispp, theme);

    // Create a white background style
    // static lv_style_t bg_style;
    // lv_style_init(&bg_style);
    // lv_style_set_bg_color(&bg_style, lv_color_black());

    // Initialize screen and set background
    ui_EndGong1_screen_init();
    // lv_obj_add_style(ui_EndGong1, &bg_style, 0);

    lv_disp_load_scr(ui_EndGong1);
    Serial.printf("Free heap: %d\n", esp_get_free_heap_size());
    Serial.printf("Largest block: %d\n", heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));


}

uint32_t count = 1;
void loop()
{
    lv_timer_handler(); /* let the GUI do its work */
    delay(5); /* let this time pass */

    // if(count == true && millis()>2000)
    // {
    //   lv_obj_clean(lv_screen_active());
    //   lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0xFF0000), LV_PART_MAIN);
    //   // lv_demo_stress();
    //   count = 0;
    // }
    // if(count++%2000 == 0)
    // {
    //  lv_obj_clean(lv_screen_active());
    // lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x0000FF), LV_PART_MAIN);
    // Serial.println( "BLUE" );    
    // }

    // else if(count++%3000== 0)
    // {
    //  lv_obj_clean(lv_screen_active());
    // lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0xFF0000), LV_PART_MAIN);
    // Serial.println( "RED" );
    // }

    // else if(count++%5000== 0)
    // {
    //  lv_obj_clean(lv_screen_active());
    // lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x00FF00), LV_PART_MAIN);
    // Serial.println( "GREEN" );
    // count = 0;
    // }

    // else if(count++%1000)
    // {
    // lv_obj_set_style_bg_color(lv_screen_active(), lv_color_white(), LV_PART_MAIN);
    // lv_obj_t *label = lv_label_create( lv_screen_active() );
    // lv_label_set_text( label, "IN LOOP, I'm LVGL!" );
    // lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    // lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );
    // }

    
}
