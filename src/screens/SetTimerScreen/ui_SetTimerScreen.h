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
extern lv_obj_t *ui_SetTimerScr;
extern lv_obj_t *ui_startBtnBox1;
extern lv_obj_t *ui_startLabel1;
extern lv_obj_t *ui_startIcon1;
extern lv_obj_t *ui_leftGong1;
extern lv_obj_t *ui_midLeftHeart1;
extern lv_obj_t *ui_midBt1;
extern lv_obj_t *ui_mainCntr;
extern lv_obj_t *ui_clkColon1;
extern lv_obj_t *ui_clkColon2;
extern lv_obj_t *ui_clkHr;
extern lv_obj_t *ui_clkSec;
extern lv_obj_t *ui_clkTopLabel;
extern lv_obj_t *ui_clkBtmLabel;
extern lv_obj_t *ui_minRoller;
extern lv_obj_t *ui_pgCntr;
extern lv_obj_t *ui_pg1;
extern lv_obj_t *ui_pg2;
extern lv_obj_t *ui_pg3;
extern lv_obj_t *ui_pg4;
extern lv_obj_t *ui_pg5;
extern lv_obj_t *ui_pgSlider;
// CUSTOM VARIABLES

#ifdef __cplusplus
}
#endif
#endif /*UI_SETTIMERSCREEN_H */