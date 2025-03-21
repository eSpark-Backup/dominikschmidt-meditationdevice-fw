idf_component_register(
    SRCS "BluetoothScreen/ui_BluetoothScreen.c"
         "EndGong/ui_EndGong.c"
         "FavEditScreen/ui_FavEditScreen.c"
         "FavPreviewScreen/ui_FavPreviewScreen.c"
         "FavScreen/ui_FavScreen.c"
         "MeditationTimeScr/ui_MeditationScreen.c"
         "ScreenHelpers/ui_helpers.c"
         "SetTimerScreen/ui_SetTimerScreen.c"
         "SoundScreen/ui_SoundScreen.c"
    INCLUDE_DIRS "." "BluetoothScreen" "EndGong" "FavEditScreen" "FavPreviewScreen" "FavScreen" "MeditationTimeScr" "ScreenHelpers" "SetTimerScreen" "SoundScreen"
    REQUIRES arduino lvgl images A2dp
)