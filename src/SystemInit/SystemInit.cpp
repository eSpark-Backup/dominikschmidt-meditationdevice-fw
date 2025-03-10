#include "SystemInit.h"

// Buttons
#define Button1_Pin 36
#define Button2_Pin 39

Button button1(Button1_Pin);
Button button2(Button2_Pin);

Button *buttons[] = {&button1, &button2};
static int numButtons;                    
static esp_timer_handle_t button_timer;   

/// Screens and Screen Groups
lv_obj_t *Screen[TotalScreens];
lv_group_t *ScreenGroup[TotalScreens];
lv_group_t *currentGroup = NULL;

/// Create system initializer object
System Core;

void System::Initialize(void)
{
    numButtons = sizeof(buttons) / sizeof(buttons[0]); // Store safely
    // ButtonsInit();                                     // No need to pass as argument
    setFocusStyle();
    initScreens();
}


void System::initScreens(void)
{
    InitializeScreen();
    loadScreens();
    loadScreenGroups();
    Serial.println("Screens initialized successfully\n\n");
}

void System::InitializeScreen(void)
{
    lv_obj_clean(lv_screen_active());
    ui_SetTimerScr_screen_init();
    ui_SoundScreen_screen_init();
    ui_EndGong1_screen_init();
    ui_FavScreen_screen_init();
    ui_FavEditScreen_screen_init();
    ui_FavPreview_screen_init();
    ui_BluetoothScreen_screen_init();
    ui_MeditationScreen_screen_init();
}

void System::loadScreens(void)
{
    Screen[0] = ui_SetTimerScreen;
    Screen[1] = ui_SoundScreen;
    Screen[2] = ui_EndGong1;
    Screen[3] = ui_FavScreen;
    Screen[4] = ui_FavEditScreen;
    Screen[5] = ui_FavPreviewScreen;
    Screen[6] = ui_BluetoothScreen;
    Screen[7] = ui_MeditationScreen;

}

void System::loadScreenGroups(void)
{
    ScreenGroup[0] = TimerScrGroup;
    ScreenGroup[1] = SoundGroup;
    ScreenGroup[2] = EndGongGroup;
    ScreenGroup[3] = FavScrGroup;
    ScreenGroup[4] = FavEditGroup;
    ScreenGroup[5] = FavPrevGroup;
    ScreenGroup[6] = BTGroup;
    ScreenGroup[7] = MeditationGroup;

}


// Function to read the button state for all buttons
void IRAM_ATTR System::checkButtons(void)
{
    for (int i = 0; i < numButtons; i++) // Use global numButtons
    {
        buttons[i]->checkState();
    }
}

void System::ButtonsInit(void)
{
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
    else if (Btn_B.State == BUTTON_SINGLE_CLICK || Btn_B.State == BUTTON_DOUBLE_CLICK || Btn_B.State == BUTTON_LONG_HOLD) {
         NavigateScreens(Btn_B);
    } 
    else 
        data->state = LV_INDEV_STATE_RELEASED; // No need to pass a key, just release
}

void System::NavigateScreens(ButtonData btnData)
{
    static int screenCount = 0;
    static bool navigatingWidgets = false;  // Tracks whether we are navigating widgets or screens
    bool screenChanged = false;
    static lv_obj_t *lastScreen = lv_screen_active();

    if(lastScreen != lv_screen_active())
    {
        navigatingWidgets = false;
        lastScreen = lv_screen_active();
        int count = lv_group_get_obj_count(currentGroup);
        if (count > 0) {
            lv_obj_t *lastObj = lv_group_get_obj_by_index(currentGroup, count - 1);
            lv_group_focus_obj(lastObj);
        }
        
        for(int curScr = 0; curScr < TotalScreens; curScr++)
        {
            if(lastScreen == Screen[curScr])
            {
                screenCount = curScr;
                lv_indev_t *indev = lv_indev_get_next(NULL);
                if (indev && lastScreen) {
                    lv_indev_set_group(indev, ScreenGroup[screenCount]);
                }
            }
        }

        Serial.println("Widget Navigation deactivated, new screen group loaded");
    }
    
    if (btnData.ID == B) // Handle Button B for selection mode
    {
        if (btnData.State == BUTTON_DOUBLE_CLICK) {  ///for testing changed to SINGLE else DOUBLE
            navigatingWidgets = !navigatingWidgets; // Exit widget navigation mode
            // Serial.printf("Widget Navigation Mode %s\n", navigatingWidgets?"Activated":"Deactivated");
            sendEvents(BUTTON_NONE, -1);
            navigateWidgets(BUTTON_NONE, screenCount, navigatingWidgets);
        }
        else if (btnData.State == BUTTON_SINGLE_CLICK || (btnData.State == BUTTON_LONG_HOLD)) {
            sendEvents(btnData.State, screenCount);
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
                lastScreen = lv_screen_active();
                
                lv_indev_t *indev = lv_indev_get_next(NULL);
                if (indev && ScreenGroup[screenCount]) {
                    lv_indev_set_group(indev, ScreenGroup[screenCount]);
                }
            }
        }
    }
}

void System::navigateWidgets(ButtonState state, int screenCount, bool focus)
{
    currentGroup = ScreenGroup[screenCount];  // Get group for the active screen
    if (!currentGroup) return; // Safety check

    if (state == BUTTON_SINGLE_CLICK) {
        lv_group_focus_next(currentGroup); // Move focus to the next widget
    } 
    else if (state == BUTTON_DOUBLE_CLICK) {
        lv_group_focus_prev(currentGroup); // Move focus to the previous widget
    }
    else if(state == BUTTON_NONE)
    {
        if(focus)
        {
            lv_group_focus_obj(lv_group_get_obj_by_index(currentGroup, 0));
        }
        else
        {
            int count = lv_group_get_obj_count(currentGroup);
            if (count > 0) {
                lv_obj_t *lastObj = lv_group_get_obj_by_index(currentGroup, count - 1);
                lv_group_focus_obj(lastObj);
            }
        }
    }
}

void System::sendEvents(ButtonState state, int screenCount)
{
    if (!currentGroup) return;  // Ensure the group exists

    lv_obj_t *focusedObj = lv_group_get_focused(currentGroup); // Get the currently focused object
    if (!focusedObj) return; // If no object is focused, exit

    lv_event_code_t event_code;
    if(state == BUTTON_LONG_HOLD && screenCount < TotalScreens)
    {
        event_code = LV_EVENT_LONG_PRESSED;
    }
    else if(state == BUTTON_SINGLE_CLICK && screenCount < TotalScreens)
    {
        event_code = LV_EVENT_PRESSED;
    }
    else if(state == BUTTON_NONE && screenCount == -1)
    {
        event_code = LV_EVENT_CANCEL;
    }
    else
    {
        return;
    }
    lv_obj_send_event(focusedObj, event_code, NULL);
}


void System::ButtonReadStatic(lv_indev_t * indev, lv_indev_data_t * data) {
    Core.ButtonRead(indev, data);
}