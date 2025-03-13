#ifndef UI_SETTIMERSCREEN_H
#define UI_SETTIMERSCREEN_H

/***************************
 *lvgl based screen designs based on mediation display theme
 ***************************/

#include "../../images/images.h"
#include "../ScreenHelpers/ui_helpers.h"

#ifdef __cplusplus
extern "C" {
#endif

// SCREEN: ui_SetTimerScreen
void ui_SetTimerScr_screen_init(void);
void ui_event_clkHr(lv_event_t * e);
void ui_event_hrRoller(lv_event_t * e);
void ui_event_clkMin(lv_event_t * e);
void ui_event_minRoller(lv_event_t * e);
void ui_event_clkSec(lv_event_t * e);
void ui_event_secRoller(lv_event_t * e);
void ui_event_resetSetTimerEvent(lv_event_t * e);
void rotateRoller(int diff);
char* setRollerOptions(int maxRange);
char* labelString(int num);
void setRollersValue();
extern lv_obj_t *ui_SetTimerScreen;
extern lv_obj_t *ui_startBtnBox1;
extern lv_obj_t *ui_startLabel1;
extern lv_obj_t *ui_startIcon1;
extern lv_obj_t *ui_rightGong;
extern lv_obj_t *ui_midRightTune;
extern lv_obj_t *ui_midTime;
extern lv_obj_t *ui_mainCntr;
extern lv_obj_t *ui_clkColon1;
extern lv_obj_t *ui_clkColon2;
extern lv_obj_t *ui_clkTopLabel;
extern lv_obj_t *ui_clkBtmLabel;
extern lv_obj_t *ui_clkHr;
extern lv_obj_t *ui_hrRoller;
extern lv_obj_t *ui_clkMin;
extern lv_obj_t *ui_minRoller;
extern lv_obj_t *ui_clkSec;
extern lv_obj_t *ui_secRoller;
extern lv_obj_t *ui_pgCntr;
extern lv_obj_t *ui_pg1;
extern lv_obj_t *ui_pg2;
extern lv_obj_t *ui_pg3;
extern lv_obj_t *ui_pg4;
extern lv_obj_t *ui_pg5;
extern lv_obj_t *ui_pgSlider;
extern lv_obj_t *ui_HiddenTimerScrBtn;
extern lv_group_t *TimerScrGroup;
extern lv_obj_t* roller;
extern bool roller_selected;
// CUSTOM VARIABLES

#ifdef __cplusplus
}
#endif
#endif /*UI_SETTIMERSCREEN_H */