#ifndef BUTTON_H
#define BUTTON_H

/***************************
 *Hardware buttons class with methods to get events like press, release, hold and double click
 ***************************/

#include <Arduino.h>

#define DEBOUNCE_TIME 30      // Debounce time in ms
#define DOUBLE_CLICK_TIME 300 // Max time between two clicks
#define SHORT_HOLD_TIME 500   // Short press-hold duration
#define LONG_HOLD_TIME 2000   // Long press-hold duration
#define POLL_INTERVAL 10      // Timer interval in milliseconds


// Enum for button states
enum ButtonState {
    BUTTON_NONE,
    BUTTON_PRESS,
    BUTTON_RELEASE,
    BUTTON_SHORT_HOLD,
    BUTTON_LONG_HOLD,
    BUTTON_SINGLE_CLICK,
    BUTTON_DOUBLE_CLICK
};


class Button
{
    private: // variables
        int _pin;
        bool _pressed;
        unsigned long _lastPressTime;
        unsigned long _pressStartTime;
        int _clickCount;
        bool _singleClickDetected;
        bool _previousSteadyState;
        bool _lastSteadyState;
        bool _lastFlickerableState;
        bool _pullUp = false;
        unsigned long _lastDebounceTime;
        ButtonState _detectedState;

    public:
        Button(int buttonPin, int mode = INPUT_PULLDOWN);
        void checkState();
        ButtonState getState();

};

#endif /*BUTTON_H */