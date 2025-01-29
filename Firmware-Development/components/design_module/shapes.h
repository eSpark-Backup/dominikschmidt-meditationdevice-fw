/**
 * @file shapes.h
 * Include all related headers and macros
 */

#ifndef SHAPES_H
#define SHAPES_H

/***************************
 *lvgl based shapes or other design elements based on mediation device theme
 ***************************/

/* Littlevgl specific */
#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#include "lvgl_helpers.h"

typedef enum {

WHITE       = 0xffffff,
BLACK       = 0x000000,
LIGHT_GREY  = 0xC0C0C0,
DAR_GREY    = 0x808080,
RED         = 0xFF0000,
ORANGE      = 0xF0A804,
YELLOW      = 0xFFFF00,
GREEN       = 0x008000,
DARK_BLUE   = 0x0000FF,
MEGENTA     = 0x800080,
} color;

void create_demo_application(void);
lv_obj_t *create_rounded_rectangle(lv_obj_t *parent, int x, int y, int width, int height, lv_color_t color);
void gumButton(void);
void simpleButton(char *displayText, color btnColor);
void createOnOffButtons(char *disp_txt1, char *disp_txt2, color btnColor);

#endif /*SHAPES_H */