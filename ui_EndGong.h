
#pragma once

#ifndef UI_ENDGONG_H
#define UI_ENDGONG_H

/***************************
 *lvgl based screen designs based on mediation display theme
 ***************************/

// #include "shapes.h"
#include "images.h"

#ifdef __cplusplus
extern "C" {
#endif
// SCREEN: ui_EndGong1
void ui_EndGong1_screen_init(void);
extern lv_obj_t *ui_EndGong1;
extern lv_obj_t *ui_startBtnBox;
extern lv_obj_t *ui_startLabel;
extern lv_obj_t *ui_startIcon;
extern lv_obj_t *ui_overlayPanel;
extern lv_obj_t *ui_onButton;
extern lv_obj_t *ui_onBtnLabel;
extern lv_obj_t *ui_offButton;
extern lv_obj_t *ui_offBtnLabel;
extern lv_obj_t *ui_endGong1Label;
extern lv_obj_t *ui_leftTime;
extern lv_obj_t *ui_RightBt;
extern lv_obj_t *ui_midLeftTune;
extern lv_obj_t *ui_midRightHeart;
extern lv_obj_t *ui_midGong;
// CUSTOM VARIABLES

#ifdef __cplusplus
}
#endif
#endif /*UI_ENDGONG_H */