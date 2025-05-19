#ifndef SYSTEMINIT_H
#define SYSTEMINIT_H

/***************************
 *Initialize system core components
 ***************************/
#include "Button.hpp"
#include "lvgl.h"
#include "RotaryEncoder.h"
#include "A2dpSource.hpp"
#include "SdCard.hpp"
#include "screens.h"
#include "esp_timer.h"
#include "driver/ledc.h"

typedef enum
{
    UNDEFINED,
    A,
    B,
}   ButtonID;

typedef struct
{
    /* data */
    ButtonID ID;
    ButtonState State; 
    
}   ButtonData;


class System
{
    //variables
    public:
    private:
    lv_obj_t *previousFocused = NULL;
    lv_obj_t *focusedObj = NULL;

    //Methods
    public:
    void InitializeScreen(void);
    void Initialize(void);
    void ButtonRead(lv_indev_t * indev, lv_indev_data_t * data);
    static void ButtonReadStatic(lv_indev_t * indev, lv_indev_data_t * data);
    void setBrightness(uint8_t brightness_percent);
    void setupPWM();
    void updateBluetoothSettings();
    void memLog();
    
    private:
    void initScreens(void);
    void loadScreens(void);
    void loadScreenGroups(void);
    void EncoderInit(void);
    void IRAM_ATTR checkButtons(void);
    static void checkEncoder(void* arg);
    void NavigateScreens(ButtonData btnData, int encData);
    void navigateWidgets(ButtonState state, int screenCount, bool focus = false);
    void sendEvents(ButtonData btn, int screenCount);
    void loadAudio();
};

extern System Core;

#endif /*SYSTEMINIT_H */