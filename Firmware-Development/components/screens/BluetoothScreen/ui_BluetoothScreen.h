#ifndef UI_BLUETOOTHSCREEN_H
#define UI_BLUETOOTHSCREEN_H

/***************************
 *lvgl based screen designs based on mediation display theme
 ***************************/

#include "images.h"
#include "ScreenHelpers/ui_helpers.h"

#ifdef __cplusplus
extern "C" {
#endif
    
// SCREEN: ui_BluetoothScreen
#define BUF_SIZE  50
void ui_BluetoothScreen_screen_init(void);
void ui_event_btFindDevicesBtn(lv_event_t * e);
void ui_event_btTap(lv_event_t * e);
void ui_event_btConnectDisconnect(lv_event_t * e);
void ui_event_btForgetDevice(lv_event_t * e);
extern lv_obj_t * ui_BluetoothScreen;
extern lv_obj_t * ui_btLabel;
extern lv_obj_t * ui_btDeviceNameLbl;
extern lv_obj_t * ui_btConnectionIcon;
extern lv_obj_t * ui_BtConnectionStatus;
extern lv_obj_t * ui_TapBtConnectLbl;
extern lv_obj_t * ui_btFindDevicesBtn;
extern lv_obj_t * ui_btFindDevicesBtnLbl;
extern lv_obj_t * ui_leftGong2;
extern lv_obj_t * ui_midLeftHeart;
extern lv_obj_t * ui_midBt;
extern lv_obj_t * ui_HiddenBTScrBtn;
extern lv_group_t * BTGroup;
extern lv_obj_t * ui_btTapBtnPanel;
extern lv_obj_t * ui_ConnectBtnCntr;
extern lv_obj_t * ui_ConnectBtnLbl;
extern lv_obj_t * ui_ConnectBtnIcon;
extern lv_obj_t * ui_ForgetDevBtnCntr;
extern lv_obj_t * ui_ForgetDevBtnLbl;
extern lv_obj_t * ui_ForgetDevBtnCntrIcon;
extern lv_obj_t * ui_BtReconnectionStatus;
// CUSTOM VARIABLES

#ifdef __cplusplus
}
#endif

#endif /*UI_BLUETOOTHSCREEN_H */