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
lv_group_t *ScreenGroup[8];

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

    ScreenGroup[0] = NULL;
    ScreenGroup[1] = NULL;
    ScreenGroup[2] = EndGongGroup;
    ScreenGroup[3] = NULL;
    ScreenGroup[4] = NULL;
    ScreenGroup[5] = NULL;
    ScreenGroup[6] = NULL;
    ScreenGroup[7] = NULL;

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
    
    if (Btn_A.State == BUTTON_SINGLE_CLICK || Btn_A.State == BUTTON_DOUBLE_CLICK) {
         NavigateScreens(Btn_A);
    } 
    if (Btn_B.State == BUTTON_SINGLE_CLICK || Btn_B.State == BUTTON_DOUBLE_CLICK) {
         NavigateScreens(Btn_B);
    } 
    data->state = LV_INDEV_STATE_RELEASED; // No need to pass a key, just release
}

void System::NavigateScreens(ButtonData btnData)
{
    static int screenCount = 0;
    static bool navigatingWidgets = false;  // Tracks whether we are navigating widgets or screens
    bool screenChanged = false;

    if (btnData.ID == B) // Handle Button B for selection mode
    {
        if (btnData.State == BUTTON_DOUBLE_CLICK) {
            navigatingWidgets = false; // Exit widget navigation mode
            Serial.println("Widget Navigation Mode Deactivated");
        }
        else if (btnData.State == BUTTON_SINGLE_CLICK) {
            navigatingWidgets = true; // Enter widget navigation mode
            Serial.println("Widget Navigation Mode Activated");
        } 
    }

    if (btnData.ID == A)
    {
        if (navigatingWidgets) {
            navigateWidgets(btnData.State, screenCount);  // Call a new function to navigate widgets
        }
        else {
            // Normal screen navigation
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
                lv_disp_load_scr(Screen[screenCount]); // Load new screen

                lv_indev_t *indev = lv_indev_get_next(NULL);
                if (indev && ScreenGroup[screenCount]) {
                    lv_indev_set_group(indev, ScreenGroup[screenCount]);
                }
            }
        }
    }
}

void System::navigateWidgets(ButtonState state, int screenCount)
{
    lv_group_t *currentGroup = ScreenGroup[screenCount];  // Get group for the active screen
    if (!currentGroup) return; // Safety check

    if (state == BUTTON_SINGLE_CLICK) {
        lv_group_focus_next(currentGroup); // Move focus to the next widget
        Serial.println("Moved to Next Widget");
    } 
    else if (state == BUTTON_DOUBLE_CLICK) {
        lv_group_focus_prev(currentGroup); // Move focus to the previous widget
        Serial.println("Moved to Previous Widget");
    }
}


void System::ButtonReadStatic(lv_indev_t * indev, lv_indev_data_t * data) {
    Core.ButtonRead(indev, data);
}