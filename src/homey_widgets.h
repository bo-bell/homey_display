#pragma once

#include <lvgl.h>

#define DEFAULT_LV_OBJ_FLAG LV_OBJ_FLAG_USER_4

void homey_styles_init(void);

lv_obj_t * homey_button_create(lv_obj_t * parent, char * text, lv_obj_flag_t flags, uint16_t height, uint16_t width );
