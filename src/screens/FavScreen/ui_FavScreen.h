#ifndef UI_FAVSCREEN_H
#define UI_FAVSCREEN_H

/***************************
 *lvgl based screen designs based on mediation display theme
 ***************************/

#include "../../images/images.h"
#include "../ScreenHelpers/ui_helpers.h"

#ifdef __cplusplus
extern "C" {
#endif

// SCREEN: ui_FavScreen
void ui_FavScreen_screen_init(void);
extern lv_obj_t * ui_FavScreen;
extern lv_obj_t * ui_FavsPanel;
extern lv_obj_t * ui_Fav1;
extern lv_obj_t * ui_LabelFav1;
extern lv_obj_t * ui_GreaterThanIcon1;
extern lv_obj_t * ui_Fav2;
extern lv_obj_t * ui_LabelFav2;
extern lv_obj_t * ui_GreaterThanIcon2;
extern lv_obj_t * ui_Fav3;
extern lv_obj_t * ui_LabelFav3;
extern lv_obj_t * ui_GreaterThanIcon3;
extern lv_obj_t * ui_Fav4;
extern lv_obj_t * ui_LabelFav4;
extern lv_obj_t * ui_GreaterThanIcon4;
extern lv_obj_t * ui_Fav5;
extern lv_obj_t * ui_LabelFav5;
extern lv_obj_t * ui_GreaterThanIcon5;
extern lv_obj_t * ui_Fav6;
extern lv_obj_t * ui_LabelFav6;
extern lv_obj_t * ui_GreaterThanIcon6;
extern lv_obj_t * ui_PanelTopShadow1;
extern lv_obj_t * ui_PanelBtmShadow1;
extern lv_obj_t * ui_leftTune;
extern lv_obj_t * ui_midLeftBt;
extern lv_obj_t * ui_midRightGong;
extern lv_obj_t * ui_midHeart;
extern lv_obj_t * ui_FavpgCntr;
extern lv_obj_t * ui_pg11;
extern lv_obj_t * ui_pg12;
extern lv_obj_t * ui_pg13;
extern lv_obj_t * ui_pg14;
extern lv_obj_t * ui_pg15;
extern lv_obj_t * ui_FavLabel;
extern lv_obj_t * ui_FavStartBtnBox;
extern lv_obj_t * ui_FavStartLabel;
extern lv_obj_t * ui_FavStartIcon;
// CUSTOM VARIABLES

#ifdef __cplusplus
}
#endif
#endif /*UI_FAVSCREEN_H */