#ifndef SCREEN_H
#define SCREEN_H

/***************************
 *lvgl based screen designs based on mediation display theme
 ***************************/
#ifdef __cplusplus
extern "C"
{
#endif

// #include "ui_EndGong.h"
// #include "ui_MeditationScreen.h"
// #include "ui_BluetoothScreen.h"
// #include "ui_SetTimerScreen.h"
// #include "ui_SoundScreen.h"
// #include "ui_FavScreen.h"
// #include "ui_FavEditScreen.h"
// #include "ui_FavPreviewScreen.h"
#include "EndGong/ui_EndGong.h"
#include "MeditationTimeScr/ui_MeditationScreen.h"
#include "BluetoothScreen/ui_BluetoothScreen.h"
#include "SetTimerScreen/ui_SetTimerScreen.h"
#include "SoundScreen/ui_SoundScreen.h"
#include "FavScreen/ui_FavScreen.h"
#include "FavEditScreen/ui_FavEditScreen.h"
#include "FavPreviewScreen/ui_FavPreviewScreen.h"
#include "EnableBt/ui_EnableConnectBt.h"
#include "A2dpSource.hpp"

extern lv_obj_t *Screen[8];
extern lv_group_t *ScreenGroup[8];


#ifdef __cplusplus
}
#endif
#endif /*SCREEN_H */