#ifndef SYSTEMINIT_H
#define SYSTEMINIT_H

/***************************
 *Initialize system core components
 ***************************/
#include "lvgl.h"
#include "Button.h"
#include "RotaryEncoder.h"
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
    
    private:
    void initScreens(void);
    void InitializeScreen(void);
    void loadScreens(void);
    void loadScreenGroups(void);
    void EncoderInit(void);
    void IRAM_ATTR checkButtons(void);
    static void IRAM_ATTR checkEncoder(void* arg);
    void NavigateScreens(ButtonData btnData);
    void navigateWidgets(ButtonState state, int screenCount, bool focus = false);
    void sendEvents(ButtonState state, int screenCount);
    void sendRollerAnimation(void);
};

extern System Core;
void memLog();

#endif /*SYSTEMINIT_H */