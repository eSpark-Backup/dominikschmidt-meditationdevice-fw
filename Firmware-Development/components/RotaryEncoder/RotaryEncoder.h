#ifndef ROTARYENCODER_H
#define ROTARYENCODER_H

/***************************
 *Rotary Encoder for Clockwise and counter Clockwise direction
 ***************************/

// #include <Arduino.h>
#include "driver/gpio.h"
// Enum for button states
enum EncoderDirection
{
    DIRECTION_NONE,
    DIRECTION_CCW,
    DIRECTION_CW
};

typedef struct
{
    EncoderDirection direction = DIRECTION_NONE;
    int counter = 0;
}   EncoderState;

class RotaryEncoder
{
    private:

        gpio_num_t _clkPin;
        gpio_num_t _dtPin;
        int _CLK_state;
        int _prev_CLK_state;
        EncoderState _encoder;

    public:
        RotaryEncoder(gpio_num_t clkPin, gpio_num_t dtPin, gpio_mode_t mode = GPIO_MODE_INPUT);
        void checkEncoderState();
        EncoderState getEncoderState();

};

#endif /*ROTARYENCODER_H */