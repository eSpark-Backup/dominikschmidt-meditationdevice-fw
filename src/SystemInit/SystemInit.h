#ifndef SYSTEMINIT_H
#define SYSTEMINIT_H

/***************************
 *Initialize system core components
 ***************************/

#include "../Button/Button.h"

class System
{
    public:
        void Initialize();
    
    private:
        void ButtonsInit();
        static void IRAM_ATTR checkButtons(void *arg);
};

#endif /*SYSTEMINIT_H */