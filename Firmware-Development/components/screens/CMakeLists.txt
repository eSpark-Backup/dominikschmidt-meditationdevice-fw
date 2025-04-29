idf_component_register(
    SRCS "BluetoothScreen/ui_BluetoothScreen.cpp"
         "EndGong/ui_EndGong.cpp"
         "FavEditScreen/ui_FavEditScreen.cpp"
         "FavPreviewScreen/ui_FavPreviewScreen.cpp"
         "FavScreen/ui_FavScreen.cpp"
         "MeditationTimeScr/ui_MeditationScreen.cpp"
         "ScreenHelpers/ui_helpers.cpp"
         "SetTimerScreen/ui_SetTimerScreen.cpp"
         "SoundScreen/ui_SoundScreen.cpp"
         "EnableBt/ui_EnableConnectBt.cpp"
    INCLUDE_DIRS "." "BluetoothScreen" "EnableBt" "EndGong" "FavEditScreen" "FavPreviewScreen" "FavScreen" "MeditationTimeScr" "ScreenHelpers" "SetTimerScreen" "SoundScreen"
    REQUIRES arduino-esp32 lvgl images
)