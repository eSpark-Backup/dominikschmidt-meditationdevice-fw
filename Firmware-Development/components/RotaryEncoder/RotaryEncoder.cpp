#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder(gpio_num_t chA_Pin, gpio_num_t chB_Pin, gpio_mode_t mode)
{
    _clkPin = chA_Pin;
    _dtPin = chB_Pin;
    gpio_set_direction(_clkPin, mode);
    gpio_set_direction(_dtPin, mode);
    _prev_CLK_state = gpio_get_level(_clkPin);
}

void RotaryEncoder::checkEncoderState()
{
    int CLK_state = gpio_get_level(_clkPin);
    int DT_state = gpio_get_level(_dtPin);

    if (CLK_state != _prev_CLK_state) {
        if (DT_state != CLK_state) {
            _encoder.counter++;
            _encoder.direction = DIRECTION_CW;
        } else {
            _encoder.counter--;
            _encoder.direction = DIRECTION_CCW;
        }
    }
    else
    {
        _encoder.direction = DIRECTION_NONE;
    }
    _prev_CLK_state = CLK_state;
}

// Function to get the detected encoder state
EncoderState RotaryEncoder::getEncoderState()
{
    return _encoder;
}