#include <Arduino.h>
#include <LovyanGFX.h>
#include <lvgl.h>
#include "Guition_480x480_driver.h"
#include "homey_widgets.h"


//LVGL display buffer
static lv_color_t buf1[screenWidth * 10];
static lv_draw_buf_t draw_buf1[screenWidth * 10]; //

static void event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        //LV_LOG_USER("Clicked");
        Serial.println("Clicked");  
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {
        //LV_LOG_USER("Toggled");
        Serial.println("Toggled");
    }
}


void init_test_screen( lv_obj_t * parent)
{
    lv_obj_t * label;

    //lv_obj_t * btn1 = lv_button_create(parent);
    lv_obj_t * btn1 = homey_button_create(parent, "Button", DEFAULT_LV_OBJ_FLAG , 160, 160);
    //lv_obj_add_style(btn1, &style_btn, 0);
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    //lv_obj_set_size(btn1, 160, 160);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -90);
    lv_obj_remove_flag(btn1, LV_OBJ_FLAG_PRESS_LOCK);

    //label = lv_label_create(btn1);
    //lv_label_set_text(label, "Button");
    //lv_obj_center(label);

    lv_obj_t * btn2 = homey_button_create(parent, "Toggle", LV_OBJ_FLAG_CHECKABLE, 160, 160);
    //lv_obj_t * btn2 = lv_button_create(parent);
    //lv_obj_add_style(btn2, &style_btn, 0);
    lv_obj_add_event_cb(btn2, event_handler, LV_EVENT_ALL, NULL);
    //lv_obj_set_size(btn2, 160, 160);
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 90);
    //lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
    //lv_obj_set_height(btn2, LV_SIZE_CONTENT);

    //label = lv_label_create(btn2);
    //lv_label_set_text(label, "Toggle");
    //lv_obj_center(label);
}

void setup()
{
    Serial.begin(115200);
    my_driver_init();
    //gfx.init();
    //gfx.begin();

    lv_init();
    Serial.println("LVGL initialized");

    lv_display_t *disp = lv_display_create( screenWidth, screenHeight);
    lv_display_set_flush_cb(disp,my_disp_flush);
    lv_display_set_buffers(disp, &draw_buf1, NULL, sizeof(draw_buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
    Serial.println("Display created and flush cb set");

    // Set innput device
    static lv_indev_t * indev_touchpad = lv_indev_create();
    lv_indev_set_type(indev_touchpad, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_touchpad, my_touchpad_read);

    // Create display content
    homey_styles_init();
    init_test_screen(lv_screen_active());

    Serial.println("Setup done");

}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  lv_tick_inc(5);
  delay(5);
}
