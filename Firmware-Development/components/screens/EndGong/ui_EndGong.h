#ifndef UI_ENDGONG_H
#define UI_ENDGONG_H

/***************************
 *lvgl based screen designs based on mediation display theme
 ***************************/

#include "images.h"
#include "ScreenHelpers/ui_helpers.h"

#ifdef __cplusplus
extern "C" {
#endif
// SCREEN: ui_EndGong1
void ui_EndGong1_screen_init(void);
void ui_event_onButton(lv_event_t * e);
void ui_event_offButton(lv_event_t * e);
void ui_event_startBtnBox(lv_event_t * e);
extern lv_obj_t *ui_EndGong1;
extern lv_obj_t *ui_endgongStartBtn;
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
extern lv_obj_t *ui_HiddenEndGongBtn;
extern lv_group_t *EndGongGroup;
extern bool startMeditation;
// CUSTOM VARIABLES

#ifdef __cplusplus
}
#endif
#endif /*UI_ENDGONG_H */