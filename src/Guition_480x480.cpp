#include <lvgl.h>
#include "Guition_480x480_driver.h"
#include "background_image.h"



static LGFX gfx; /*Use the default LovyanGFX display class instance*/

//LVGL display buffer
static lv_color_t buf1[screenWidth * 10];
static lv_draw_buf_t draw_buf1[screenWidth * 10]; //

void my_lv_and_driver_init(bool use_bg_image)
{
    gfx.init();
    lv_init();
    
    lv_display_t *disp = lv_display_create( screenWidth, screenHeight);
    lv_display_set_flush_cb(disp,my_disp_flush);
    lv_display_set_buffers(disp, &draw_buf1, NULL, sizeof(draw_buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    lv_obj_t * screen = lv_scr_act();
    if (use_bg_image) {
        lv_obj_t * img = lv_img_create(screen);
        lv_img_set_src(img, &background_image);
    }
    //lv_img_set_src(img, &lysbryter_paa);
    Serial.println("Display created and flush cb set");

    // Set innput device
    static lv_indev_t * indev_touchpad = lv_indev_create();
    lv_indev_set_type(indev_touchpad, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_touchpad, my_touchpad_read);
}

//Flush callback
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
    //Serial.println("Flush called");
    gfx.pushImage(area->x1, area->y1,
                  area->x2 - area->x1 + 1,
                  area->y2 - area->y1 + 1,
                 (lgfx::rgb565_t *)px_map);
    lv_display_flush_ready(disp);
}

void my_touchpad_read( lv_indev_t * indev_driver, lv_indev_data_t * data )
{
    
    uint16_t touchX, touchY;

    data->state = LV_INDEV_STATE_REL;

    if( gfx.getTouch( &touchX, &touchY ) )
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = touchX;
        data->point.y = touchY;
        // Serial.printf("Touch at %d,%d\n", touchX, touchY);
    }
}