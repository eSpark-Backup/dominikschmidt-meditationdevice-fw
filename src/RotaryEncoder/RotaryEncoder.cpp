#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder(int chA_Pin, int chB_Pin, int mode)
{
    _clkPin = chA_Pin;
    _dtPin = chB_Pin;
    pinMode(_clkPin, mode);
    pinMode(_dtPin, mode);
    _prev_CLK_state = digitalRead(_clkPin);
}

void RotaryEncoder::checkEncoderState()
{
    int CLK_state = digitalRead(_clkPin);
    int DT_state = digitalRead(_dtPin);

    if (CLK_state != _prev_CLK_state) {
        if (DT_state != CLK_state) {
            _encoder.counter++;
            _encoder.direction = DIRECTION_CW;
        } else {
            _encoder.counter--;
            _encoder.direction = DIRECTION_CCW;
        }
        // printf("Encoder value: %d - %d\n", _encoder.counter, _encoder.direction);
    }
    _prev_CLK_state = CLK_state;
}

// Function to get the detected encoder state
EncoderState RotaryEncoder::getEncoderState()
{
    return _encoder;
}