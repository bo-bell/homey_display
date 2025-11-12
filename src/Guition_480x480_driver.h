#pragma once

#include "lvgl.h"
#include "LGFX_ESP32S3_RGB_TFT_SPI_ST7701_GT911.h"

/*Change to your screen resolution*/
#define screenWidth   480
#define screenHeight  480


//Flush callback
void my_driver_init(void);
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map);
void my_touchpad_read( lv_indev_t * indev_driver, lv_indev_data_t * data );
