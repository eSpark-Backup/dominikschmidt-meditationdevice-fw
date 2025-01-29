
#include "screen_design.h"
#include "esp_log.h"

#define TAG "Screen_Design"

void endGong1(void)
{
    /* Change the active screen's background color */
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_black(), LV_PART_MAIN);
    /* Create a rounded rectangle without a label */
    create_rounded_rectangle(lv_screen_active(), 20, 190, 200, 100, lv_color_white());

    createOnOffButtons("On", "Off", LIGHT_GREY);
}
