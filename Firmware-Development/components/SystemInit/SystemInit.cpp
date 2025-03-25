#include "SystemInit.h"
#include "esp_log.h"

const char* TAG = "SystemInit";

// Buttons
#define Button1_Pin GPIO_NUM_36
#define Button2_Pin GPIO_NUM_39

Button button1(Button1_Pin);
Button button2(Button2_Pin);

Button *buttons[] = {&button1, &button2};
static int numButtons;

//Rotary Encoder
#define ENC_CHA GPIO_NUM_25 // Encoder's CLK pin
#define ENC_CHB GPIO_NUM_26 // Encoder's DT pin
RotaryEncoder Enc(ENC_CHA, ENC_CHB);
static esp_timer_handle_t enc_timer;
int encDiff = 0;

/// Screens and Screen Groups
lv_obj_t *Screen[TotalScreens];
lv_group_t *ScreenGroup[TotalScreens];
lv_group_t *currentGroup = NULL;
bool navigatingWidgets = false;  // Tracks whether we are navigating widgets or screens


// LEDC Configuration
#define PWM_CHANNEL    LEDC_CHANNEL_0
#define PWM_FREQUENCY  5000               // 5 kHz
#define PWM_RESOLUTION LEDC_TIMER_10_BIT  // 10-bit resolution (0-1023)
#define PWM_PIN        GPIO_NUM_27        // BKL GPIO

/// Create system initializer object
System Core;

void System::Initialize(void)
{
    numButtons = sizeof(buttons) / sizeof(buttons[0]);
    EncoderInit();
    setFocusStyle();
    initScreens();
    btAudio.A2dpServiceInit();
}

void System::initScreens(void)
{
    InitializeScreen();
    loadScreens();
    loadScreenGroups();
    ESP_LOGI(TAG, "Screens initialized successfully\n\n");
}

void System::EncoderInit()
{
    esp_timer_create_args_t timer_args = {
        .callback = &checkEncoder,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "enc_timer"};

    esp_err_t ret = esp_timer_create(&timer_args, &enc_timer);
    ret |= esp_timer_start_periodic(enc_timer, POLL_INTERVAL * 1000); // Convert ms to µs

    if (ret == ESP_OK)
    {
    ESP_LOGI(TAG, "enc_timer initialized");
    }
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

void System::ButtonRead(lv_indev_t * indev, lv_indev_data_t * data)
{    
    static int last_encoder_counter = 0;
    EncoderState enc = Enc.getEncoderState();
    ButtonData Btn_A, Btn_B;
    int newCounter = enc.counter;
    encDiff = newCounter - last_encoder_counter;
    last_encoder_counter = newCounter;
    
    if(encDiff != 0)
    {
        if ((roller_selected && roller != NULL) || list_selected)  // Only update if roller is in selection mode
        {
            if (roller_selected)        {rotateRoller(encDiff);return;}
            else if (list_selected)     {rotateDropdown(encDiff);return;}
        }
        if (!roller_selected || !list_selected) {
            Btn_A.ID = UNDEFINED;
            NavigateScreens(Btn_A, encDiff);
        }
    }
    else
    {
        checkButtons();
        Btn_A = {A, button1.getState()};
        Btn_B = {B, button2.getState()};
        
        if (Btn_A.State == BUTTON_SINGLE_CLICK || Btn_A.State == BUTTON_DOUBLE_CLICK) {
            NavigateScreens(Btn_A, encDiff);
        } 
        else if (Btn_B.State == BUTTON_SINGLE_CLICK || Btn_B.State == BUTTON_DOUBLE_CLICK || Btn_B.State == BUTTON_LONG_HOLD) {
            NavigateScreens(Btn_B, encDiff);
        } 
        else 
            data->state = LV_INDEV_STATE_RELEASED; // No need to pass a key, just release
    }
}

void System::NavigateScreens(ButtonData btnData, int encData)
{
    static int screenCount = 0;
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
    }
    if(btAudio.isAudioStarted() && screenCount != 6)
    {
        btAudio.A2dpAudioStop();
    }
    if(encDiff != 0 && !navigatingWidgets)
    {
        // Normal screen navigation
        screenCount += encDiff;
        screenChanged = true;

        if (screenCount > 7) screenCount = 0;
        if (screenCount < 0) screenCount = 7;

        if (screenChanged) {
            lv_disp_load_scr(Screen[screenCount]); // Load new screen
            lastScreen = lv_screen_active();
            
            lv_indev_t *indev = lv_indev_get_next(NULL);
            if (indev && ScreenGroup[screenCount]) {
                lv_indev_set_group(indev, ScreenGroup[screenCount]);
                lv_obj_send_event(Screen[screenCount], LV_EVENT_CANCEL, NULL);
            }
        }
        if(screenCount == 6)
        {
            btAudio.start_bt_scan();
        }
    }
    else if (btnData.ID == B) // Handle Button B for selection mode
    {
        if (btnData.State == BUTTON_DOUBLE_CLICK) {  ///for testing changed to SINGLE else DOUBLE
            navigatingWidgets = !navigatingWidgets; // Exit widget navigation mode
            sendEvents(BUTTON_NONE, -1);
            navigateWidgets(BUTTON_NONE, screenCount, navigatingWidgets);
        }
        else if (btnData.State == BUTTON_SINGLE_CLICK || (btnData.State == BUTTON_LONG_HOLD)) {
            sendEvents(btnData.State, screenCount);
        } 
    }
    else if (btnData.ID == A)
    {
        if (navigatingWidgets) {
            navigateWidgets(btnData.State, screenCount);  // Call a new function to navigate widgets
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

    static lv_obj_t *previousFocused = NULL;
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
        // if(screenCount == 6)
        // {
        //     static int countStart = 0; // testcode
        //     ESP_LOGE(TAG, "startA2dpService : %d", countStart);
        //          if(countStart == 0)     btAudio.A2dpAudioStart("/sdcard/converted/test.mp3");
        //     else if(countStart == 1)     btAudio.A2dpAudioStart("/sdcard/converted/Stream 1.wav", 300);
        //     else if(countStart == 2)     btAudio.A2dpAudioStart("/sdcard/converted/Birds 2.wav", 100);
        //     else if(countStart == 3)     btAudio.A2dpAudioStart("/sdcard/converted/Stream 2.wav", 50);
        //     else if(countStart == 4)     btAudio.A2dpAudioStart("/sdcard/converted/Thunder 1.wav", 450);
        //     else if(countStart == 5)     btAudio.A2dpAudioStart("/sdcard/converted/Birds 1.wav");
        //     else                         btAudio.A2dpAudioStart("/sdcard/converted/Thunder 2.wav");
        //     countStart++;
        //     if(countStart>=6) countStart = 0;
        // }
    }
    else if(state == BUTTON_NONE && screenCount == -1)
    {
        event_code = LV_EVENT_CANCEL;
    }
    else
    {
        return;
    }
    if(previousFocused && previousFocused != focusedObj)
    {
        lv_obj_send_event(previousFocused, LV_EVENT_DEFOCUSED, NULL);
    }

    lv_obj_send_event(focusedObj, event_code, NULL);
    
    previousFocused = focusedObj;
}

void System::ButtonReadStatic(lv_indev_t * indev, lv_indev_data_t * data) {
    Core.ButtonRead(indev, data);
}

void System::checkEncoder(void* arg)
{
    Enc.checkEncoderState();
}

// Function to map brightness percentage (50%-100%) to 10-bit duty cycle (512-1024)
void System::setBrightness(uint8_t brightness_percent)
{
    if (brightness_percent < 50) brightness_percent = 50;  // Min 50%
    if (brightness_percent > 100) brightness_percent = 100; // Max 100%

    uint32_t duty = (brightness_percent * 1024) / 100;  // Map to 10-bit range

    // Set brightness (duty cycle)
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL, duty);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL);
}

void System::setupPWM()
{
    // Configure LEDC timer
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_HIGH_SPEED_MODE,
        .duty_resolution  = PWM_RESOLUTION,
        .timer_num        = LEDC_TIMER_0,
        .freq_hz          = PWM_FREQUENCY,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // Configure LEDC channel
    ledc_channel_config_t ledc_channel = {
        .gpio_num       = PWM_PIN,
        .speed_mode     = LEDC_HIGH_SPEED_MODE,
        .channel        = PWM_CHANNEL,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER_0,
        .duty           = 1024,  // Default to 100% brightness
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);
    
    // Set initial brightness to 50%
    setBrightness(50);
}

uint32_t getCurrentDuty()
{
    return (ledc_get_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL)*100)/1024;
}
void memLog()
{
    ESP_LOGI(TAG,
        "\n"
        "FREE_HEAP           : %7zu bytes\n"
        "LARGEST_BLOCK       : %7zu bytes\n"
        "MALLOC_CAP_8BIT     : %7zu bytes\n"
        "MALLOC_CAP_DMA      : %7zu bytes\n"
        "MALLOC_CAP_SPIRAM   : %7zu bytes\n"
        "MALLOC_CAP_INTERNAL : %7zu bytes\n"
        "MALLOC_CAP_DEFAULT  : %7zu bytes\n"
        "CURRENT DUTY SET    : %d%% approx\n",
        esp_get_free_heap_size(),
        heap_caps_get_largest_free_block(MALLOC_CAP_8BIT),
        heap_caps_get_free_size(MALLOC_CAP_8BIT),
        heap_caps_get_free_size(MALLOC_CAP_DMA),
        heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
        heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
        heap_caps_get_free_size(MALLOC_CAP_DEFAULT),
        getCurrentDuty());    
}
