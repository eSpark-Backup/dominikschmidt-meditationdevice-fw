#include "RotaryEncoder.h"

#define ENCODER_DEBOUNCE_TIME 50  // Debounce time in milliseconds

RotaryEncoder::RotaryEncoder(gpio_num_t chA_Pin, gpio_num_t chB_Pin, gpio_mode_t mode)
{
    _clkPin = chA_Pin;
    _dtPin = chB_Pin;
    gpio_set_direction(_clkPin, mode);
    gpio_set_direction(_dtPin, mode);
    _prev_CLK_state = gpio_get_level(_clkPin);
    _lastUpdateTime = esp_timer_get_time() / 1000; // Initialize timer in milliseconds
}

void RotaryEncoder::checkEncoderState()
{
    int CLK_state = gpio_get_level(_clkPin);
    int DT_state = gpio_get_level(_dtPin);
    
    // Get the current time in milliseconds
    uint32_t currentTime = esp_timer_get_time() / 1000;

    // Only update if debounce time has passed
    if (currentTime - _lastUpdateTime > ENCODER_DEBOUNCE_TIME)
    {
        if (CLK_state != _prev_CLK_state) {
            if (DT_state != CLK_state) {
                _encoder.counter++;
                _encoder.direction = DIRECTION_CW;
            } else {
                _encoder.counter--;
                _encoder.direction = DIRECTION_CCW;
            }
            _lastUpdateTime = currentTime;  // Reset debounce timer
        }
        else
        {
            _encoder.direction = DIRECTION_NONE;
        }
        _prev_CLK_state = CLK_state;
    }
}

// Function to get the detected encoder state
EncoderState RotaryEncoder::getEncoderState()
{
    return _encoder;
}

// RotaryEncoder::RotaryEncoder(gpio_num_t chA_Pin, gpio_num_t chB_Pin, gpio_mode_t mode)
// {
//     _clkPin = chA_Pin;
//     _dtPin = chB_Pin;
//     gpio_set_direction(_clkPin, mode);
//     gpio_set_direction(_dtPin, mode);
//     _prev_CLK_state = gpio_get_level(_clkPin);
// }

// void RotaryEncoder::checkEncoderState()
// {
//     int CLK_state = gpio_get_level(_clkPin);
//     int DT_state = gpio_get_level(_dtPin);

//     if (CLK_state != _prev_CLK_state) {
//         if (DT_state != CLK_state) {
//             _encoder.counter++;
//             _encoder.direction = DIRECTION_CW;
//         } else {
//             _encoder.counter--;
//             _encoder.direction = DIRECTION_CCW;
//         }
//     }
//     else
//     {
//         _encoder.direction = DIRECTION_NONE;
//     }
//     _prev_CLK_state = CLK_state;
// }

// // Function to get the detected encoder state
// EncoderState RotaryEncoder::getEncoderState()
// {
//     return _encoder;
// }