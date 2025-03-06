#ifndef UI_MEDITATIONTIMESCREEN_H
#define UI_MEDITATIONTIMESCREEN_H

/***************************
 *lvgl based screen designs based on mediation display theme
 ***************************/

#include "../../images/images.h"
#include "../ScreenHelpers/ui_helpers.h"

#ifdef __cplusplus
extern "C" {
#endif

// SCREEN: ui_MeditationScreen
void ui_MeditationScreen_screen_init(void);
extern lv_obj_t *ui_MeditationScreen;
extern lv_obj_t *ui_meditationTimer;
extern lv_obj_t *ui_ArcOverlay;
extern lv_obj_t *ui_medTimeRemain;
extern lv_obj_t *ui_medTimePassed;
void ui_event_medPauseBtn(lv_event_t *e);
extern lv_obj_t *ui_medPauseBtn;
extern lv_obj_t *ui_HiddenMedScrBtn;
extern lv_group_t *MeditationGroup;
// CUSTOM VARIABLES

#ifdef __cplusplus
}
#endif
#endif /*UI_MEDITATIONTIMESCREEN_H */