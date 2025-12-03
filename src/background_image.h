#pragma once

#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif

extern const lv_image_dsc_t background_image;

extern const lv_image_dsc_t lysbryter_av_100x100;
extern const lv_image_dsc_t lysbryter_paa_100x100;
extern const lv_image_dsc_t lysbryter_av_150x150;
extern const lv_image_dsc_t lysbryter_paa_150x150;
extern const lv_image_dsc_t lysbryter_av_200x200;
extern const lv_image_dsc_t lysbryter_paa_200x200;
extern const lv_image_dsc_t lysbryter_av_400x400;
extern const lv_image_dsc_t lysbryter_paa_400x400;

