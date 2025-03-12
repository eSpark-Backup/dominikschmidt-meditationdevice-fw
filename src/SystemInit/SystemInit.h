#ifndef SYSTEMINIT_H
#define SYSTEMINIT_H

/***************************
 *Initialize system core components
 ***************************/
#include "lvgl.h"
#include "../Button/Button.h"
#include "../RotaryEncoder/RotaryEncoder.h"
#include "../screens/screens.h"
#include "esp_timer.h"

typedef enum ButtonID
{
    UNDEFINED,
    A,
    B,
};

typedef struct ButtonData
{
    /* data */
    ButtonID ID;
    ButtonState State; 

};


class System
{
    public:
        void Initialize(void);
        void ButtonRead(lv_indev_t * indev, lv_indev_data_t * data);
        static void ButtonReadStatic(lv_indev_t * indev, lv_indev_data_t * data);
        
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
};

extern System Core;
#endif /*SYSTEMINIT_H */