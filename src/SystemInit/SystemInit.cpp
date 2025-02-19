#include "SystemInit.h"

// Buttons
#define Button1_Pin 5
#define Button2_Pin 18

Button button1(Button1_Pin);
Button button2(Button2_Pin);

Button *buttons[] = {&button1, &button2}; // Define buttons globally
static int numButtons;                    // Store button count safely
static esp_timer_handle_t button_timer;   // Ensure it's globally accessible

void System::Initialize()
{
    numButtons = sizeof(buttons) / sizeof(buttons[0]); // Store safely
    ButtonsInit();                                     // No need to pass as argument
}

// Function to read the button state for all buttons
void IRAM_ATTR System::checkButtons(void *arg)
{
    for (int i = 0; i < numButtons; i++) // Use global numButtons
    {
        buttons[i]->checkState();
    }
}

void System::ButtonsInit()
{
    esp_timer_create_args_t timer_args = {
        .callback = &checkButtons,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "button_timer"};

    esp_err_t ret = esp_timer_create(&timer_args, &button_timer);
    ret |= esp_timer_start_periodic(button_timer, POLL_INTERVAL * 1000); // Convert ms to µs

    if (ret == ESP_OK)
        Serial.println("Buttons initialized");
}
