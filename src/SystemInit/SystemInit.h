#ifndef SYSTEMINIT_H
#define SYSTEMINIT_H

/***************************
 *Initialize system core components
 ***************************/
#include "lvgl.h"
#include "../Button/Button.h"
#include "../screens/screens.h"

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
        void Initialize();
        void ButtonRead(lv_indev_t * indev, lv_indev_data_t * data);
        static void ButtonReadStatic(lv_indev_t * indev, lv_indev_data_t * data);
        
        private:
        void initScreens();
        void ButtonsInit();
        void IRAM_ATTR checkButtons();
        void NavigateScreens(ButtonData btnData);
};

extern System Core;

#endif /*SYSTEMINIT_H */