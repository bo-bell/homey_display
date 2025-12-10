#include <lvgl.h>
#include "homey_widgets.h"
#include "homey_styles.h"
#include "background_image.h"
#include <Arduino.h>

lv_obj_t * homey_panel(lv_obj_t * parent, uint16_t width, uint16_t height,
                       lv_align_t align, int32_t x_ofs, int32_t y_ofs)
{

    lv_obj_t * panel = lv_obj_create(parent);
    lv_obj_remove_style_all(panel);
    lv_obj_add_style(panel, &homey_panel_style, LV_PART_MAIN);
    
    lv_obj_set_size(panel, width, height);
    lv_obj_align(panel, align,  x_ofs, y_ofs);
    lv_obj_add_flag(panel,LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(panel,LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_remove_flag(panel,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(panel,LV_OBJ_FLAG_SCROLLABLE);

    return panel;
}

lv_obj_t * homey_button_create(lv_obj_t * parent, const char * text, lv_obj_flag_t flags = DEFAULT_LV_OBJ_FLAG , uint16_t height = -1, uint16_t width = -1)
{
    lv_obj_t * btn = lv_button_create(parent);
    if ( flags != DEFAULT_LV_OBJ_FLAG )  lv_obj_add_flag(btn, flags );
    lv_obj_add_flag(btn, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_EVENT_BUBBLE);


    /*
    if ( height != -1 && width != -1 )
    if ( text != NULL )  {
        lv_obj_set_size(btn, width, height);
        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text(label, text);
        lv_obj_center(label);
    }
    */
    lv_obj_add_style(btn, &homey_btn_style, LV_PART_MAIN);
    lv_obj_set_size(btn,100,100);

    return btn;
}


void homey_light_switch_event_cb(lv_event_t * e)
{
    lv_obj_t * obj = (lv_obj_t*) lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);

    //if      ( code == LV_EVENT_CLICKED)       Serial.println("Homey_light_switch : CLICKED");
    //else if ( code == LV_EVENT_PRESS_LOST)    Serial.println("Homey_light_switch : PRESS_LOST");
    //else if ( code == LV_EVENT_RELEASED)      Serial.println("Homey_light_switch : RELEAST");
    //else if ( code == LV_EVENT_GESTURE)       Serial.println("Homey_light_switch : GESTURE");
    //else if ( code == LV_EVENT_VALUE_CHANGED) Serial.println("Homey_light_switch : VALUE_CHANGED");
    
    lv_obj_t * img = lv_obj_get_child(obj,0); // the image is the first child

    if( lv_obj_has_state(obj, LV_STATE_CHECKED) ) {
      //  lv_image_set_scale(img, 192); // 75%
        lv_image_set_src(img, &lysbryter_paa_400x400);
    } else {
      //  lv_image_set_scale(img, 128); // 50%
        lv_image_set_src(img,&lysbryter_av_400x400);
    }

}

lv_obj_t *  homey_light_switch( lv_obj_t * parent, const char * text, 
                                 lv_obj_flag_t flags = DEFAULT_LV_OBJ_FLAG , 
                                 uint16_t size_x = 200, 
                                 uint16_t size_y = 200  
                               )
{
    /* Supporting 4 sizes:  100x100, 150x150, 200x200, 400x400 */ 

    lv_obj_t * button = lv_button_create(parent);
    lv_obj_remove_style_all(button);
    lv_obj_add_style(button, &homey_btn_style, LV_PART_MAIN);

    lv_obj_set_size(button,size_x,size_y);
    lv_obj_t * img = lv_image_create(button);
    lv_img_set_src(img, &lysbryter_paa_400x400);
    lv_obj_center(img);

    // calculate image scaling ; 256 = 100%
    uint16_t image_scale_x = (size_x * 256) / 400;  // 400 is original image size
    uint16_t image_scale_y = (size_y * 256) / 400;

    // zoom image to correct siz, 256  = 100%
    lv_image_set_scale_x(img,image_scale_x);
    lv_image_set_scale_y(img,image_scale_y);

    if ( text != NULL )  {
        //height = lv_obj_get_content_height()
        //width = lv_obj_get_content_width()
        lv_obj_t * label = lv_label_create(button);
        lv_label_set_text(label, text);
        lv_obj_align(label, LV_ALIGN_CENTER, 0 , -30);
    }

    lv_obj_add_flag(button, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_flag(button, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_add_flag(button, LV_OBJ_FLAG_EVENT_BUBBLE);

    //lv_obj_add_event_cb(imgbtn, homey_light_switch_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(button, homey_light_switch_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(button, homey_light_switch_event_cb, LV_EVENT_PRESS_LOST, NULL);
    lv_obj_add_event_cb(button, homey_light_switch_event_cb, LV_EVENT_RELEASED, NULL);
    lv_obj_add_event_cb(button, homey_light_switch_event_cb, LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(button, homey_light_switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    //lv_obj_align(imgbtn1, LV_ALIGN_CENTER, 0, 0);
    return button;
}

