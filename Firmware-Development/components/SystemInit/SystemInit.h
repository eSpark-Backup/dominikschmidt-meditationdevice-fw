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
    public:
    void Initialize(void);
    void ButtonRead(lv_indev_t * indev, lv_indev_data_t * data);
    static void ButtonReadStatic(lv_indev_t * indev, lv_indev_data_t * data);
    void setBrightness(uint8_t brightness_percent);
    void setupPWM();
    void updateBluetoothSettings();
    
    private:
    void initScreens(void);
    void InitializeScreen(void);
    void loadScreens(void);
    void loadScreenGroups(void);
    void EncoderInit(void);
    void IRAM_ATTR checkButtons(void);
    static void checkEncoder(void* arg);
    void NavigateScreens(ButtonData btnData, int encData);
    void navigateWidgets(ButtonState state, int screenCount, bool focus = false);
    void sendEvents(ButtonState state, int screenCount);
    void loadAudio();
};

extern System Core;
void memLog();

#endif /*SYSTEMINIT_H */