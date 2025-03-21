#include "Button.hpp"


Button::Button(gpio_num_t buttonPin, gpio_mode_t mode)
{
    _pin = buttonPin;
    _pressed = false;
    _lastPressTime = 0;
    _pressStartTime = 0;
    _clickCount = 0;
    _singleClickDetected = false;
    _pullUp = mode == GPIO_MODE_INPUT ? false : true;
    _lastSteadyState = true; // Default state for INPUT_PULLUP
    _lastFlickerableState = _lastSteadyState;
    _lastDebounceTime = 0;
    _detectedState = BUTTON_NONE;
    gpio_set_direction(_pin, mode);
}

void Button::checkState()
{
    unsigned long currentTime = xTaskGetTickCount();
    int currentState = gpio_get_level(_pin);

    // Debounce logic
    if (currentState != _lastFlickerableState)
    {
        _lastDebounceTime = currentTime;
        _lastFlickerableState = currentState;
    }
    
    // Reset detected state each cycle
    _detectedState = BUTTON_NONE;

    if ((currentTime - _lastDebounceTime) >= DEBOUNCE_TIME)
    {
        _previousSteadyState = _lastSteadyState;
        _lastSteadyState = currentState;

        // Button Pressed
        if (_previousSteadyState == _pullUp && _lastSteadyState == !_pullUp)
        {
            _pressStartTime = xTaskGetTickCount();
            _pressed = true;
            _lastPressTime = xTaskGetTickCount();
            _clickCount++;
            // Serial.printf("Button %d Pressed\n", _pin);
            _detectedState = BUTTON_PRESS;
        }

        // Button Released
        if (_previousSteadyState == !_pullUp && _lastSteadyState == _pullUp)
        {
            _pressed = false;
            unsigned long pressDuration = xTaskGetTickCount() - _pressStartTime;
            // Serial.printf("Button %d Released\n", _pin);

            // Prioritize long hold first
            if (pressDuration >= LONG_HOLD_TIME)
            {
                // Serial.printf("Button %d Long Hold Detected\n", _pin);
                _clickCount = 0;
                _detectedState = BUTTON_LONG_HOLD;
            }
            else if (pressDuration >= SHORT_HOLD_TIME)
            {
                // Serial.printf("Button %d Short Hold Detected\n", _pin);
                _clickCount = 0;
                _detectedState = BUTTON_SHORT_HOLD;
            }
            else
            {
                _detectedState = BUTTON_RELEASE;
            }
        }

        // Double Click Detection (higher priority than single click)
        if (_clickCount == 1 && !_singleClickDetected)
        {
            _singleClickDetected = true;
            _lastPressTime = xTaskGetTickCount();
        }

        if (_clickCount == 2)
        {
            // Serial.printf("Button %d Double Click Detected\n", _pin);
            _clickCount = 0;
            _singleClickDetected = false;
            _detectedState = BUTTON_DOUBLE_CLICK;
        }

        // Single Click Detection (only happens if double click doesn't happen)
        if (_singleClickDetected && (xTaskGetTickCount() - _lastPressTime > DOUBLE_CLICK_TIME))
        {
            // Serial.printf("Button %d Single Click Detected\n", _pin);
            _clickCount = 0;
            _singleClickDetected = false;
            _detectedState = BUTTON_SINGLE_CLICK;
        }
    }
}

// Function to get the detected button state
ButtonState Button::getState()
{
    return _detectedState; // Return the highest-priority detected event
}

