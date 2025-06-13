#ifndef UI_SOUNDSCREEN_H
#define UI_SOUNDSCREEN_H

/***************************
 *lvgl based screen designs based on mediation display theme
 ***************************/

#include "images.h"
#include "ScreenHelpers/ui_helpers.h"

#ifdef __cplusplus
extern "C" {
#endif

// SCREEN: ui_SoundScreen
void ui_SoundScreen_screen_init(void);
void ui_event_toggleSoundPanelGroup(lv_event_t * e);
void ui_event_NoSoundBtn(lv_event_t * e);
void ui_event_NatureSoundBtn(lv_event_t * e);
void ui_event_WhiteNoiseBtn(lv_event_t * e);
void ui_event_BinauralBeatBtn(lv_event_t * e);
void ui_event_LessonsBtn(lv_event_t * e);
void ui_event_SingingBowlsBtn(lv_event_t * e);
extern lv_obj_t * ui_SoundScreen;
extern lv_obj_t * ui_SoundPanel;
extern lv_obj_t * ui_NoSoundBtn;
extern lv_obj_t * ui_NoSoundCb;
extern lv_obj_t * ui_NoSoundIcon;
extern lv_obj_t * ui_NoSoundLabel;
extern lv_obj_t * ui_NatureSoundBtn;
extern lv_obj_t * ui_NatureSoundCb;
extern lv_obj_t * ui_NatureSoundIcon;
extern lv_obj_t * ui_NatureSoundLbl;
extern lv_obj_t * ui_NatureSoundDropList;
extern lv_obj_t * ui_WhiteNoiseDropList;
extern lv_obj_t * ui_BinauralBeatDropList;
extern lv_obj_t * ui_LessonsDropList;
extern lv_obj_t * ui_WhiteNoiseBtn;
extern lv_obj_t * ui_WhiteNoiseCb;
extern lv_obj_t * ui_WhiteNoiseIcon;
extern lv_obj_t * ui_WhiteNoiseLbl;
extern lv_obj_t * ui_BinauralBeatBtn;
extern lv_obj_t * ui_BinauralBeatCb;
extern lv_obj_t * ui_BinauralBeatIcon;
extern lv_obj_t * ui_BinauralBeatLbl;
extern lv_obj_t * ui_SingingBowlsBtn;
extern lv_obj_t * ui_SingingBowlsCb;
extern lv_obj_t * ui_SingingBowlsIcon;
extern lv_obj_t * ui_SingingBowlsLbl;
extern lv_obj_t * ui_PanelTopShadow;
extern lv_obj_t * ui_PanelBtmShadow;
extern lv_obj_t * ui_leftheart;
extern lv_obj_t * ui_midLeftGong;
extern lv_obj_t * ui_midRightTimer;
extern lv_obj_t * ui_midTune;
extern lv_obj_t * ui_SoundpgCntr;
extern lv_obj_t * ui_pgCntr2;
extern lv_obj_t * ui_pg6;
extern lv_obj_t * ui_pg7;
extern lv_obj_t * ui_pg8;
extern lv_obj_t * ui_pg9;
extern lv_obj_t * ui_pg10;
extern lv_obj_t * ui_SoundStartBtnBox;
extern lv_obj_t * ui_SoundStartLabel;
extern lv_obj_t * ui_SoundStartIcon;
extern lv_obj_t * ui_SoundLabel;
extern lv_obj_t *ui_HiddenSoundBtn;
extern lv_group_t *SoundGroup;
// CUSTOM VARIABLES

typedef enum
{
    NONE_SELECTED,
    NO_SOUND,
    NATURE_SOUND,
    WHITE_NOISE,
    BINAURAL_BEATS,
    LESSONS

} SoundType;

extern SoundType selSoundOpt;

#ifdef __cplusplus
}
#endif
#endif /*UI_SOUNDSCREEN_H */