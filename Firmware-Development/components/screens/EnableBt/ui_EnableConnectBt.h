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
extern lv_obj_t * ui_Spinner;
extern lv_obj_t * ui_btScanLbl;
extern lv_obj_t * ui_midBt1;
extern lv_obj_t * ui_btEnableLabel;
extern lv_obj_t * ui_btEnablePanel;
extern lv_obj_t * ui_btEnableBtn;
extern lv_obj_t * ui_btEnableBtnIcon;
extern lv_obj_t * ui_btEnableBtnLabel;
extern lv_obj_t * ui_btEnableSwitch;
extern lv_obj_t * ui_ScannedDevDropdown;
extern lv_obj_t * ui_curDropdownList;
extern lv_obj_t * ui_btScanBtn;
extern lv_obj_t * ui_btScanBtnIcon;
extern lv_obj_t * ui_btScanBtnLabel;
extern lv_obj_t * ui_btConnectBtnBox;
extern lv_obj_t * ui_btConnectLabel;
extern lv_obj_t * ui_HiddenEnableBtScrBtn;
extern lv_group_t * EnableBtGroup;
void ui_event_btEnableSwitch(lv_event_t * e);
void ui_event_btListSwitch(lv_event_t * e);
void ui_event_btScanBtn(lv_event_t * e);
void ui_event_commonEvents(lv_event_t * e);
void ui_event_btConnectBtn(lv_event_t * e);
void EnableBtFocus(bool focus);
void rotateScannedDropdown(int diff);
void checkBtDeviceConnectivity();
void switchFocus(lv_obj_t *btn);
void resetScanScrParams();
lv_obj_t* style_device_button();
lv_obj_t* style_device_label(lv_obj_t *btn, const char* text);
extern bool list_selected;
#define NAME_BUF  50
extern char deviceName[NAME_BUF];
#define MAX_SCANNED_DEVICES 6
extern lv_obj_t * ui_btDevice[MAX_SCANNED_DEVICES];
// CUSTOM VARIABLES

#ifdef __cplusplus
}
#endif
#endif /*UI_ENABLECONNECTBT_H */