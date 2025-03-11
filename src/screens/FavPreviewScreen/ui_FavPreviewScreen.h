#ifndef UI_FAVPREVIEWSCREEN_H
#define UI_FAVPREVIEWSCREEN_H

/***************************
 *lvgl based screen designs based on mediation display theme
 ***************************/

#include "../../images/images.h"
#include "../ScreenHelpers/ui_helpers.h"

#ifdef __cplusplus
extern "C" {
#endif

// SCREEN: ui_FavPreviewScreen
void ui_FavPreview_screen_init(void);
void ui_event_FavEditTimeEvent(lv_event_t * e);
void ui_event_FavEditTuneEvent(lv_event_t * e);
void ui_event_FavEditSoundEvent(lv_event_t * e);
void ui_event_FavEditGongEvent(lv_event_t * e);
extern lv_obj_t *ui_FavPreviewScreen;
extern lv_obj_t *ui_FavPreviewLabel;
extern lv_obj_t *ui_FavEditTimeInfo;
extern lv_obj_t *ui_FavEditTimeIcon;
extern lv_obj_t *ui_FavEditTimeIconLbl;
extern lv_obj_t *ui_FavEditTimeLbl;
extern lv_obj_t *ui_FavEditSoundTypeInfo;
extern lv_obj_t *ui_FavEditTuneIcon;
extern lv_obj_t *ui_FavEditTuneIconLbl;
extern lv_obj_t *ui_FavEditTuneLbl;
extern lv_obj_t *ui_FavEditSoundInfo;
extern lv_obj_t *ui_FavEditSoundIcon;
extern lv_obj_t *ui_FavEditSoundIconLbl;
extern lv_obj_t *ui_FavEditSoundLvl;
extern lv_obj_t *ui_FavEditGongInfo;
extern lv_obj_t *ui_FavEditGongIcon;
extern lv_obj_t *ui_FavEditGongIconLbl;
extern lv_obj_t *ui_FavEditGongStatus;
extern lv_obj_t *ui_FavEditStartBtn;
extern lv_obj_t *ui_FavEditStartBtnLbl;
extern lv_obj_t *ui_FavEditStartBtnIcon;
extern lv_obj_t *ui_HiddenFavPrevBtn;
extern lv_group_t *FavPrevGroup;
// CUSTOM VARIABLES

#ifdef __cplusplus
}
#endif
#endif /*UI_FAVPREVIEWSCREEN_H */