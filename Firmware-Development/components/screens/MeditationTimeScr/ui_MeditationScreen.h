#ifndef UI_MEDITATIONTIMESCREEN_H
#define UI_MEDITATIONTIMESCREEN_H

/***************************
 *lvgl based screen designs based on mediation display theme
 ***************************/

#include "images.h"
#include "ScreenHelpers/ui_helpers.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#ifdef __cplusplus
extern "C" {
#endif


#define MIN_TO_SEC(x)           (x*60)
#define HRS_TO_SEC(x)           (x*60*MIN_TO_SEC(x))
#define SEC_TO_MIN(x)           (x/60)
#define SEC_TO_HRS(x)           (x/(60*SEC_TO_MIN(x)))
#define TIME_IN_SEC             (xTaskGetTickCount()/1000)
#define MIN_FROM_SEC(x)         (x / 60)
#define SEC_REMAIN(x)           (x % 60)

// SCREEN: ui_MeditationScreen
void ui_MeditationScreen_screen_init(void);
void ui_event_medPauseBtn(lv_event_t * e);
void ui_event_medScreenEvent(lv_event_t * e);
void timerIsUp(bool isFalse, bool navTrue);
void setTimeLabels(uint32_t remainingTime, uint32_t passedTime);
void setTimerValues(uint32_t time, bool startTimer);
void controlMedTimer();
void updateTimer();
void pauseMeditationTimer();
extern lv_obj_t *ui_MeditationScreen;
extern lv_obj_t *ui_meditationTimer;
extern lv_obj_t *ui_ArcOverlay;
extern lv_obj_t *ui_medTimeRemain;
extern lv_obj_t *ui_medTimePassed;
extern lv_obj_t *ui_medPauseBtn;
extern lv_obj_t *ui_HiddenMedScrBtn;
extern lv_timer_t *meditation_timer;
extern lv_group_t *MeditationGroup;
extern unsigned long startTime;
extern bool timerRunning;
// CUSTOM VARIABLES

#ifdef __cplusplus
}
#endif
#endif /*UI_MEDITATIONTIMESCREEN_H */