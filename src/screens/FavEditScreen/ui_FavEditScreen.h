#ifndef UI_FAVEDITSCREEN_H
#define UI_FAVEDITSCREEN_H

/***************************
 *lvgl based screen designs based on mediation display theme
 ***************************/

#include "../../images/images.h"
#include "../ScreenHelpers/ui_helpers.h"

#ifdef __cplusplus
extern "C" {
#endif

// SCREEN: ui_FavEditScreen
void ui_FavEditScreen_screen_init(void);
extern lv_obj_t * ui_FavEditScreen;
extern lv_obj_t * ui_FavEditLabel;
extern lv_obj_t * ui_EditBtn;
extern lv_obj_t * ui_EditBtnLabel;
extern lv_obj_t * ui_EditBtnIcon;
extern lv_obj_t * ui_DeleteBtn;
extern lv_obj_t * ui_DeleteBtnLabel;
extern lv_obj_t * ui_DeleteBtnIcon;
// CUSTOM VARIABLES

#ifdef __cplusplus
}
#endif
#endif /*UI_FAVEDITSCREEN_H */