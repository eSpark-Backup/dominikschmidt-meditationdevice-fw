#ifndef SCREEN_H
#define SCREEN_H

/***************************
 *lvgl based screen designs based on mediation display theme
 ***************************/
#ifdef __cplusplus
extern "C"
{
#endif

#include "EndGong/ui_EndGong.h"
#include "MeditationTimeScr/ui_MeditationScreen.h"
#include "BluetoothScreen/ui_BluetoothScreen.h"
#include "SetTimerScreen/ui_SetTimerScreen.h"
#include "SoundScreen/ui_SoundScreen.h"
#include "FavScreen/ui_FavScreen.h"
#include "FavEditScreen/ui_FavEditScreen.h"
#include "FavPreviewScreen/ui_FavPreviewScreen.h"



typedef struct screenMenu
{
    lv_obj_t *prevScr;
    lv_obj_t *curScr;
    lv_obj_t *nextScr;
};


#ifdef __cplusplus
}
#endif
#endif /*SCREEN_H */