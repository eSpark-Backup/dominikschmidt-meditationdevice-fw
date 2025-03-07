#ifndef UI_BLUETOOTHSCREEN_H
#define UI_BLUETOOTHSCREEN_H

/***************************
 *lvgl based screen designs based on mediation display theme
 ***************************/

#include "../../images/images.h"
#include "../ScreenHelpers/ui_helpers.h"

#ifdef __cplusplus
extern "C" {
#endif

// SCREEN: ui_BluetoothScreen
void ui_BluetoothScreen_screen_init(void);
void ui_event_btBtn(lv_event_t * e);
extern lv_obj_t * ui_BluetoothScreen;
extern lv_obj_t * ui_startBtnBox2;
extern lv_obj_t * ui_startLabel2;
extern lv_obj_t * ui_startIcon2;
extern lv_obj_t * ui_btBtn;
extern lv_obj_t * ui_btBtnIcon;
extern lv_obj_t * ui_btBtnLabel;
extern lv_obj_t * ui_btLabel2;
extern lv_obj_t * ui_leftGong2;
extern lv_obj_t * ui_midLeftHeart;
extern lv_obj_t * ui_midBt;
extern lv_obj_t *ui_HiddenBTScrBtn;
extern lv_group_t *BTGroup;
// CUSTOM VARIABLES

#ifdef __cplusplus
}
#endif
#endif /*UI_BLUETOOTHSCREEN_H */