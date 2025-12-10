#pragma once

#include <lvgl.h>

#define DEFAULT_LV_OBJ_FLAG LV_OBJ_FLAG_USER_4

void homey_styles_init(void);

lv_obj_t * homey_panel(lv_obj_t * parent, uint16_t width, uint16_t height,
                       lv_align_t align, int32_t x_ofs, int32_t y_ofs);

lv_obj_t * homey_button_create(lv_obj_t * parent, const char * text, lv_obj_flag_t flags, uint16_t height, uint16_t width );

lv_obj_t * homey_light_switch  ( lv_obj_t * parent, const char * text, lv_obj_flag_t flags, 
                                  uint16_t size_x,  uint16_t size_y
                                );  
                        
extern lv_style_t style_button_red;

