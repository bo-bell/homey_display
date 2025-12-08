#include <lvgl.h>
#include "homey_widgets.h"
#include "background_image.h"
#include <Arduino.h>




// Styles
static lv_style_t style_btn;
static lv_style_t style_button_pressed;
lv_style_t style_button_red;
static lv_style_t my_btn_style;
static lv_style_t style_def;

//
static lv_color_t darken(const lv_color_filter_dsc_t * dsc, lv_color_t color, lv_opa_t opa)
{
    LV_UNUSED(dsc);
    //return lv_color_darken(color, opa);
    return lv_color_make(0,255,0);
}

void homey_styles_init(void)
{
    /*Create a simple button style*/
    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, 10);
    lv_style_set_bg_opa(&style_btn, LV_OPA_COVER);
    lv_style_set_bg_color(&style_btn, lv_palette_lighten(LV_PALETTE_GREY, 3));
    lv_style_set_bg_grad_color(&style_btn, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_bg_grad_dir(&style_btn, LV_GRAD_DIR_VER);

    lv_style_set_border_color(&style_btn, lv_color_black());
    lv_style_set_border_opa(&style_btn, LV_OPA_20);
    lv_style_set_border_width(&style_btn, 2);

    lv_style_set_text_color(&style_btn, lv_color_black());
    // Set font for this style (add this line)
    lv_style_set_text_font(&style_btn, &lv_font_montserrat_44);


    /*Create a style for the pressed state.
     *Use a color filter to simply modify all colors in this state*/
    static lv_color_filter_dsc_t color_filter;
    lv_color_filter_dsc_init(&color_filter, darken);
    lv_style_init(&style_button_pressed);
    lv_style_set_color_filter_dsc(&style_button_pressed, &color_filter);
    lv_style_set_color_filter_opa(&style_button_pressed, LV_OPA_20);

    /*Create a red style. Change only some colors.*/
    lv_style_init(&style_button_red);
    lv_style_set_bg_color(&style_button_red, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_bg_grad_color(&style_button_red, lv_palette_lighten(LV_PALETTE_RED, 3));



    lv_style_init(&style_def);
    lv_style_set_text_color(&style_def, lv_color_white());
    //lv_style_set_transition(&style_def, &tr);
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
    lv_obj_add_style(btn, &my_btn_style, LV_PART_MAIN);
    lv_obj_set_size(btn,100,100);

    return btn;
}


void homey_light_switch_event_cb(lv_event_t * e)
{
    lv_obj_t * obj = (lv_obj_t*) lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);
    uint16_t * btn_size = (uint16_t *) lv_obj_get_user_data(obj);

    if      ( code == LV_EVENT_CLICKED)       Serial.println("Homey_light_switch : CLICKED");
    else if ( code == LV_EVENT_PRESS_LOST)    Serial.println("Homey_light_switch : PRESS_LOST");
    else if ( code == LV_EVENT_RELEASED)      Serial.println("Homey_light_switch : RELEAST");
    else if ( code == LV_EVENT_GESTURE)       Serial.println("Homey_light_switch : GESTURE");
    else if ( code == LV_EVENT_VALUE_CHANGED) Serial.println("Homey_light_switch : VALUE_CHANGED");
    
 
    if( lv_obj_has_state(obj, LV_STATE_CHECKED) ) {
        if ( *btn_size == 100 ) {
            lv_imgbtn_set_src(obj, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &lysbryter_paa_100x100, NULL);
        } else if ( *btn_size == 150 ) {
            lv_imgbtn_set_src(obj, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &lysbryter_paa_150x150, NULL);    
        } else if ( *btn_size == 400 ) {
            lv_imgbtn_set_src(obj, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &lysbryter_paa_400x400, NULL);
        } else {
            lv_imgbtn_set_src(obj, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &lysbryter_paa_200x200, NULL);
        }
    } else {
        if ( *btn_size == 100 ) {
            lv_imgbtn_set_src(obj, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &lysbryter_av_100x100, NULL);
        } else if ( *btn_size == 150 ) {
            lv_imgbtn_set_src(obj, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &lysbryter_av_150x150, NULL);    
        } else if ( *btn_size == 400 ) {
            lv_imgbtn_set_src(obj, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &lysbryter_av_400x400, NULL);
        } else {
            lv_imgbtn_set_src(obj, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &lysbryter_av_200x200, NULL);
        }       
    }

    if ( code == LV_EVENT_RELEASED) {

    }
}

static uint16_t button_size_100 = 100;
static uint16_t button_size_150 = 150;
static uint16_t button_size_200 = 200;
static uint16_t button_size_400 = 400;


lv_obj_t *  homey_light_switch(lv_obj_t * parent, const char * text, lv_obj_flag_t flags = DEFAULT_LV_OBJ_FLAG , uint16_t size = 200)
{
    /* Supporting 4 sizes:  100x100, 150x150, 200x200, 400x400 */ 


    lv_obj_t * imgbtn = lv_imgbtn_create(parent);
    lv_obj_add_flag(imgbtn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_flag(imgbtn, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_add_flag(imgbtn, LV_OBJ_FLAG_EVENT_BUBBLE);

    if ( size == 400 ) {
        lv_imgbtn_set_src(imgbtn, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &lysbryter_paa_400x400, NULL);
        lv_imgbtn_set_src(imgbtn, LV_IMAGEBUTTON_STATE_PRESSED, NULL, &lysbryter_av_400x400 , NULL);
        lv_obj_set_user_data(imgbtn, &button_size_400);
    }       
    else if ( size == 100 ) {
        lv_imgbtn_set_src(imgbtn, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &lysbryter_paa_100x100, NULL);
        lv_imgbtn_set_src(imgbtn, LV_IMAGEBUTTON_STATE_PRESSED, NULL, &lysbryter_av_100x100 , NULL);
        lv_obj_set_user_data(imgbtn, &button_size_100);
    } else if ( size == 150 ) {
        lv_imgbtn_set_src(imgbtn, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &lysbryter_paa_150x150, NULL);
        lv_imgbtn_set_src(imgbtn, LV_IMAGEBUTTON_STATE_PRESSED, NULL, &lysbryter_av_150x150 , NULL);
        lv_obj_set_user_data(imgbtn, &button_size_150);
    } else {
        lv_imgbtn_set_src(imgbtn, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &lysbryter_paa_200x200, NULL);
        lv_imgbtn_set_src(imgbtn, LV_IMAGEBUTTON_STATE_PRESSED, NULL, &lysbryter_av_200x200, NULL);
        lv_obj_set_user_data(imgbtn, &button_size_200);
    }


    if ( text != NULL )  {
        //height = lv_obj_get_content_height()
        //width = lv_obj_get_content_width()
        lv_obj_t * label = lv_label_create(imgbtn);
        lv_label_set_text(label, text);
        lv_obj_align(label, LV_ALIGN_CENTER, 0 , -30);
    }
    
    //lv_obj_add_event_cb(imgbtn, homey_light_switch_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(imgbtn, homey_light_switch_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(imgbtn, homey_light_switch_event_cb, LV_EVENT_PRESS_LOST, NULL);
    lv_obj_add_event_cb(imgbtn, homey_light_switch_event_cb, LV_EVENT_RELEASED, NULL);
    lv_obj_add_event_cb(imgbtn, homey_light_switch_event_cb, LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(imgbtn, homey_light_switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    //lv_obj_align(imgbtn1, LV_ALIGN_CENTER, 0, 0);
    return imgbtn;
}