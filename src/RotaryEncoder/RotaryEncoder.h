#ifndef ROTARYENCODER_H
#define ROTARYENCODER_H

/***************************
 *Rotary Encoder for Clockwise and counter Clockwise direction
 ***************************/

#include <Arduino.h>

// Enum for button states
enum EncoderDirection
{
    DIRECTION_NONE,
    DIRECTION_CCW,
    DIRECTION_CW
};

typedef struct EncoderState
{
    EncoderDirection direction = DIRECTION_NONE;
    int counter = 0;
};

class RotaryEncoder
{
    private:

        int _clkPin;
        int _dtPin;
        int _CLK_state;
        int _prev_CLK_state;
        EncoderState _encoder;

    public:
        RotaryEncoder(int clkPin, int dtPin, int mode = INPUT_PULLUP);
        void checkEncoderState();
        EncoderState getEncoderState();

};

#endif /*ROTARYENCODER_H */