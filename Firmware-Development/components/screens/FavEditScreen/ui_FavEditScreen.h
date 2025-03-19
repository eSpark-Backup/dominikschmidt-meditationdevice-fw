#ifndef UI_FAVEDITSCREEN_H
#define UI_FAVEDITSCREEN_H

/***************************
 *lvgl based screen designs based on mediation display theme
 ***************************/

#include "images.h"
#include "ScreenHelpers/ui_helpers.h"

#ifdef __cplusplus
extern "C" {
#endif

// SCREEN: ui_FavEditScreen
void ui_FavEditScreen_screen_init(void);
void ui_event_EditBtn(lv_event_t * e);
void ui_event_DeleteBtn(lv_event_t * e);
extern lv_obj_t * ui_FavEditScreen;
extern lv_obj_t * ui_FavEditLabel;
extern lv_obj_t * ui_EditBtn;
extern lv_obj_t * ui_EditBtnLabel;
extern lv_obj_t * ui_EditBtnIcon;
extern lv_obj_t * ui_DeleteBtn;
extern lv_obj_t * ui_DeleteBtnLabel;
extern lv_obj_t * ui_DeleteBtnIcon;
extern lv_obj_t * ui_HiddenBtn;
extern lv_group_t *FavEditGroup;
extern char FavEditLblBuffer[25];
// CUSTOM VARIABLES

#ifdef __cplusplus
}
#endif
#endif /*UI_FAVEDITSCREEN_H */