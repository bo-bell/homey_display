#include <lvgl.h>
#include "Guition_480x480_driver.h"



static LGFX gfx; /*Use the default LovyanGFX display class instance*/

void my_driver_init(void)
{
    gfx.init();
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
        Serial.printf("Touch at %d,%d\n", touchX, touchY);
    }
}