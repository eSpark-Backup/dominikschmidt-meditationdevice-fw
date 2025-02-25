#include "SystemInit.h"

// Buttons
#define Button1_Pin 5
#define Button2_Pin 18

Button button1(Button1_Pin);
Button button2(Button2_Pin);

Button *buttons[] = {&button1, &button2}; // Define buttons globally
static int numButtons;                    // Store button count safely
static esp_timer_handle_t button_timer;   // Ensure it's globally accessible
lv_obj_t *Screen[8];

/// Create system initializer object
System Core;

void System::Initialize()
{
    numButtons = sizeof(buttons) / sizeof(buttons[0]); // Store safely
    // ButtonsInit();                                     // No need to pass as argument
    initScreens();
}


void System::initScreens()
{
    lv_obj_clean(lv_screen_active());
    ui_EndGong1_screen_init();
    ui_MeditationScreen_screen_init();
    ui_BluetoothScreen_screen_init();
    ui_SetTimerScr_screen_init();
    ui_SoundScreen_screen_init();
    ui_FavScreen_screen_init();
    ui_FavEditScreen_screen_init();
    ui_FavPreview_screen_init();
    
    Screen[0] = ui_SetTimerScreen;
    Screen[1] = ui_SoundScreen;
    Screen[2] = ui_EndGong1;
    Screen[3] = ui_FavScreen;
    Screen[4] = ui_FavEditScreen;
    Screen[5] = ui_FavPreviewScreen;
    Screen[6] = ui_BluetoothScreen;
    Screen[7] = ui_MeditationScreen;

    Serial.println("Screens initialized successfully\n\n");
}

// Function to read the button state for all buttons
void IRAM_ATTR System::checkButtons()
{
    for (int i = 0; i < numButtons; i++) // Use global numButtons
    {
        buttons[i]->checkState();
    }
}

void System::ButtonsInit()
{
    // esp_timer_create_args_t timer_args = {
    //     .callback = &checkButtons,
    //     .arg = NULL,
    //     .dispatch_method = ESP_TIMER_TASK,
    //     .name = "button_timer"};

    // esp_err_t ret = esp_timer_create(&timer_args, &button_timer);
    // ret |= esp_timer_start_periodic(button_timer, POLL_INTERVAL * 2000); // Convert ms to µs

    // if (ret == ESP_OK)
        Serial.println("Buttons initialized");
}


void System::ButtonRead(lv_indev_t * indev, lv_indev_data_t * data)
{    
    checkButtons();
    ButtonData Btn_A = {A, button1.getState()};
    ButtonData Btn_B = {B, button2.getState()};
    
    // Handle screen navigation
    if (Btn_A.State != BUTTON_NONE) {
         NavigateScreens(Btn_A);
    } 
    data->state = LV_INDEV_STATE_RELEASED; // No need to pass a key, just release
}

void System::NavigateScreens(ButtonData btnData)
{
    static int screenCount = 0;
    bool screenChanged = false;

    if (btnData.ID == A)
    {
        if (btnData.State == BUTTON_DOUBLE_CLICK) {
            screenCount--;
            screenChanged = true;
        }
        else if (btnData.State == BUTTON_SINGLE_CLICK) {
            screenCount++;
            screenChanged = true;
        } 

        // Wrap around
        if (screenCount > 7) screenCount = 0;
        if (screenCount < 0) screenCount = 7;

        if (screenChanged) {
            lv_disp_load_scr(Screen[screenCount]);// Load new screen
        }
    }
    else if(btnData.ID == B)
    {
        if (btnData.State == BUTTON_SINGLE_CLICK) {
           
        } 
        else if (btnData.State == BUTTON_DOUBLE_CLICK) {
           
        }
    }

}

void System::ButtonReadStatic(lv_indev_t * indev, lv_indev_data_t * data) {
    Core.ButtonRead(indev, data);
}