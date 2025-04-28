#ifndef UI_ENABLECONNECTBT_H
#define UI_ENABLECONNECTBT_H

/***************************
 *lvgl based screen designs based on mediation display theme
 ***************************/

#include "images.h"
#include "ScreenHelpers/ui_helpers.h"

#ifdef __cplusplus
extern "C" {
#endif

// SCREEN: ui_EnableConnectBt
void ui_EnableConnectBt_screen_init(void);
extern lv_obj_t * ui_EnableConnectBt;
extern lv_obj_t * ui_leftGong1;
extern lv_obj_t * ui_midLeftHeart1;
extern lv_obj_t * ui_midBt1;
extern lv_obj_t * ui_btEnableLabel;
extern lv_obj_t * ui_btEnablePanel;
extern lv_obj_t * ui_btEnableBtn;
extern lv_obj_t * ui_btEnableBtnIcon;
extern lv_obj_t * ui_btEnableBtnLabel;
void ui_event_btEnableSwitch(lv_event_t * e);
extern lv_obj_t * ui_btEnableSwitch;
extern lv_obj_t * ui_ScannedDevDropdown;
extern lv_obj_t * ui_btScanBtn;
extern lv_obj_t * ui_btScanBtnIcon;
extern lv_obj_t * ui_btScanBtnLabel;
extern lv_obj_t * ui_btConnectBtnBox;
extern lv_obj_t * ui_btConnectLabel;
// CUSTOM VARIABLES

#ifdef __cplusplus
}
#endif
#endif /*UI_ENABLECONNECTBT_H */