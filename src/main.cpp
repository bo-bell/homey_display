#include <Arduino.h>
#include <LovyanGFX.h>
#include <lvgl.h>
#include "LGFX_ESP32S3_RGB_TFT_SPI_ST7701_GT911.h"

/*Change to your screen resolution*/
static const uint16_t screenWidth  = 480;
static const uint16_t screenHeight = 480;

static LGFX gfx; /*Use the default LovyanGFX display class instance*/

//LVGL display buffer
static lv_color_t buf1[screenWidth * 10];
static lv_draw_buf_t draw_buf1[screenWidth * 10]; //

// Styles
static lv_style_t style_btn;
static lv_style_t style_button_pressed;
static lv_style_t style_button_red;


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

//
static lv_color_t darken(const lv_color_filter_dsc_t * dsc, lv_color_t color, lv_opa_t opa)
{
    LV_UNUSED(dsc);
    //return lv_color_darken(color, opa);
    return lv_color_make(0,255,0);
}

static void style_init(void)
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
}

void init_test_screen( lv_obj_t * parent)
{
    lv_obj_t * label;

    lv_obj_t * btn1 = lv_button_create(parent);
    lv_obj_add_style(btn1, &style_btn, 0);
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_size(btn1, 160, 160);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -90);
    lv_obj_remove_flag(btn1, LV_OBJ_FLAG_PRESS_LOCK);

    label = lv_label_create(btn1);
    lv_label_set_text(label, "Button");
    lv_obj_center(label);

    lv_obj_t * btn2 = lv_button_create(parent);
    lv_obj_add_style(btn2, &style_btn, 0);
    lv_obj_add_event_cb(btn2, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_size(btn2, 160, 160);
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 90);
    lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
    //lv_obj_set_height(btn2, LV_SIZE_CONTENT);

    label = lv_label_create(btn2);
    lv_label_set_text(label, "Toggle");
    lv_obj_center(label);
}

void setup()
{
    Serial.begin(115200);
    gfx.init();
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
    style_init();
    init_test_screen(lv_screen_active());

    Serial.println("Setup done");

}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  lv_tick_inc(5);
  delay(5);
}
