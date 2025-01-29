
#include "shapes.h"
#include "esp_log.h"

#define TAG "Shapes"

void gumButton(void)
{
    /*Properties to transition*/
    static lv_style_prop_t props[] = {
        LV_STYLE_TRANSFORM_WIDTH, LV_STYLE_TRANSFORM_HEIGHT, LV_STYLE_TEXT_LETTER_SPACE, 0};

    /*Transition descriptor when going back to the default state.
     *Add some delay to be sure the press transition is visible even if the press was very short*/
    static lv_style_transition_dsc_t transition_dsc_def;
    lv_style_transition_dsc_init(&transition_dsc_def, props, lv_anim_path_overshoot, 250, 100, NULL);

    /*Transition descriptor when going to pressed state.
     *No delay, go to presses state immediately*/
    static lv_style_transition_dsc_t transition_dsc_pr;
    lv_style_transition_dsc_init(&transition_dsc_pr, props, lv_anim_path_ease_in_out, 250, 0, NULL);

    /*Add only the new transition to he default state*/
    static lv_style_t style_def;
    lv_style_init(&style_def);
    lv_style_set_transition(&style_def, &transition_dsc_def);

    /*Add the transition and some transformation to the presses state.*/
    static lv_style_t style_pr;
    lv_style_init(&style_pr);
    lv_style_set_transform_width(&style_pr, 10);
    lv_style_set_transform_height(&style_pr, -10);
    lv_style_set_text_letter_space(&style_pr, 10);
    lv_style_set_transition(&style_pr, &transition_dsc_pr);

    lv_obj_t *btn1 = lv_btn_create(lv_scr_act());
    lv_obj_set_style_bg_color(btn1, lv_color_hex(LIGHT_GREY), LV_PART_MAIN);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -80);
    lv_obj_add_style(btn1, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn1, &style_def, 0);

    lv_obj_t *label = lv_label_create(btn1);
    lv_label_set_text(label, "Gum");
}

void simpleButton(char* displayText, color btnColor)
{
    lv_obj_t *btn = lv_btn_create(lv_scr_act());                          // Create button
    lv_obj_set_size(btn, 100, 50);                                        // Set size
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);                             // Align in the center
    lv_obj_set_style_bg_color(btn, lv_color_hex(btnColor), LV_PART_MAIN); // Set background color
    lv_obj_t *label = lv_label_create(btn); // Create label inside the button
    lv_label_set_text(label, displayText);
    lv_obj_center(label); // Center label inside the button
}

void createOnOffButtons(char *disp_txt1, char *disp_txt2, color btnColor)
{
    // Create ON button
    lv_obj_t *btn_on = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn_on, 80, 50);                                         // Set size
    lv_obj_align(btn_on, LV_ALIGN_LEFT_MID, 30, 0);                          // Position it towards left
    lv_obj_set_style_bg_color(btn_on, lv_color_hex(btnColor), LV_PART_MAIN); // Green color for ON
    lv_obj_set_style_border_width(btn_on, 0, LV_PART_MAIN);
    lv_obj_set_style_outline_width(btn_on, 0, LV_PART_MAIN);

    lv_obj_t *label_on = lv_label_create(btn_on);
    lv_label_set_text(label_on, disp_txt1);
    lv_obj_center(label_on);

    // Create OFF button and align it to the right of ON button
    lv_obj_t *btn_off = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn_off, 80, 50);
    lv_obj_align_to(btn_off, btn_on, LV_ALIGN_OUT_RIGHT_MID, 20, 0);          // Align next to ON button
    lv_obj_set_style_bg_color(btn_off, lv_color_hex(btnColor), LV_PART_MAIN); // Red color for OFF
    lv_obj_set_style_border_width(btn_off, 0, LV_PART_MAIN);
    lv_obj_set_style_outline_width(btn_off, 0, LV_PART_MAIN);

    lv_obj_t *label_off = lv_label_create(btn_off);
    lv_label_set_text(label_off, disp_txt2);
    lv_obj_center(label_off);
}

/* Function to create a rounded rectangle */
lv_obj_t *create_rounded_rectangle(lv_obj_t *parent, int x, int y, int width, int height, lv_color_t color)
{
    /* Create the rectangle */
    lv_obj_t *rect = lv_obj_create(parent);
    lv_obj_set_size(rect, width, height); // Set width and height
    lv_obj_set_pos(rect, x, y);           // Set position

    // Disable scrolling on both parent and child
    lv_obj_clear_flag(parent, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(rect, LV_OBJ_FLAG_SCROLLABLE);

    // Ensure no scrollbar mode is applied
    lv_obj_set_scrollbar_mode(parent, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scrollbar_mode(rect, LV_SCROLLBAR_MODE_OFF);

    /* Style the rectangle */
    static lv_style_t style_rect;
    lv_style_init(&style_rect);
    lv_style_set_radius(&style_rect, 20);      // Rounded corners
    lv_style_set_bg_color(&style_rect, color); // Background color
    lv_style_set_border_width(&style_rect, 0); // No border

    lv_obj_add_style(rect, &style_rect, 0);

    return rect; // Return the rectangle object
}

void create_demo_application(void)
{
    /* Change the active screen's background color */
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);

    /* Create a white label, set its text and align it to the center */
    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "LVGL-TEST\nSUCCESSFUL!"); // "\n" ensures "Successful!" is on the next line

    /* Set the text font and color */
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x000000), LV_PART_MAIN);

    /* Align the label text and the label itself */
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);           // Ensure wrapping is handled correctly
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0); // Center align the text within the label
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);                  // Align the label to the center of the screen

    ESP_LOGI(TAG, "Setup done");
}