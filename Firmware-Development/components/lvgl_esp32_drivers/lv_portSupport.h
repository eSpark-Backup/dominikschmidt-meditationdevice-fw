/**
 * @file lv_portSupport.h
 * Include all related headers and macros
 */

#ifndef LV_PORTSUPPORT_H
#define LV_PORTSUPPORT_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************
 * Manual lvgl port support for compatibility with new lvgl releases
 ***************************/

#include "src/lvgl_private.h"

#define LV_HOR_RES_MAX 240
#define LV_VER_RES_MAX 240
#define SPI_HOST_MAX 2

typedef struct _lv_display_t lv_disp_drv_t;
typedef struct _lv_indev_t lv_indev_drv_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_PORTSUPPORT_H */