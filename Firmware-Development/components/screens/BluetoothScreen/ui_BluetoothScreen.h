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
void ui_event_connectBtnBox(lv_event_t * e);
void ui_event_btBtn(lv_event_t * e);
void ui_event_btDropdownList(lv_event_t * e);
void rotateDropdown(int diff);
extern lv_obj_t * ui_BluetoothScreen;
extern lv_obj_t * ui_connectBtnBox;
extern lv_obj_t * ui_connectLbl;
extern lv_obj_t * ui_connectIcon;
extern lv_obj_t * ui_btBtn;
extern lv_obj_t * ui_btBtnIcon;
extern lv_obj_t * ui_btBtnLabel;
extern lv_obj_t * ui_btLabel;
extern lv_obj_t * ui_leftGong2;
extern lv_obj_t * ui_midLeftHeart;
extern lv_obj_t * ui_midBt;
extern lv_obj_t * ui_btDropdownList;
extern lv_obj_t * ui_HiddenBTScrBtn;
extern lv_group_t * BTGroup;
// CUSTOM VARIABLES

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
inline lv_obj_flag_t operator|(lv_obj_flag_t lhs, lv_obj_flag_t rhs) {
    return static_cast<lv_obj_flag_t>((int)lhs | (int)rhs);
}
#endif

#endif /*UI_BLUETOOTHSCREEN_H */