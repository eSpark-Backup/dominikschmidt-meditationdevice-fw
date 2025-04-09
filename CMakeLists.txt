SET(SOURCES screens/ui_EndGong1.c
    screens/ui_MeditationScreen.c
    screens/ui_BlutoothScreen.c
    screens/ui_SetTimerScreen.c
    screens/ui_SoundScreen.c
    screens/ui_FavScreen.c
    screens/ui_FavPreviewScreen.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_21_blackstarticon_png.c
    images/ui_img_52_blacktimeicon_png.c
    images/ui_img_52_blackbticon_png.c
    images/ui_img_52_blacktuneicon_png.c
    images/ui_img_52_blackhearticon_png.c
    images/ui_img_52_blackgongicon_png.c
    images/ui_img_arcspaces230x240_png.c
    images/ui_img_playbutton_png.c
    images/ui_img_52_pauseicon_png.c
    images/ui_img_52_blacknosoundicon_png.c
    images/ui_img_52_blackbirdicon_png.c
    images/ui_img_52_whitenoiseicon_png.c
    images/ui_img_52_binauralbeats_png.c
    images/ui_img_52_singingbowls_png.c
    images/ui_img_52_greaterthanicon_png.c
    images/ui_img_20_smalltuneicon_png.c
    images/ui_img_20_smallgongicon_png.c)

add_library(ui ${SOURCES})
