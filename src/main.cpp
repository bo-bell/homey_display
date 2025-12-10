#include <Arduino.h>
#include <LovyanGFX.h>
#include <lvgl.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
//#include <ESPAsyncTCP.h>
#include <ArduinoJson.h>
#include "Guition_480x480_driver.h"
#include "homey_widgets.h"
#include "homey_styles.h"


static lv_obj_t * panel_main;
static lv_obj_t * panel_pageing;

int current_page = 0;
int requested_page = 1;

typedef struct {
    bool gesture_handled;
    bool guesture_in_progress;
} gesture_guard_t;

static gesture_guard_t gesture_guard = {
    .gesture_handled = false,
    .guesture_in_progress = false,  
};

static void next_page() { 
    requested_page = requested_page + 1; 
    Serial.printf("Increasing page number to : %d\n",requested_page);
}

static void previous_page() { 
    requested_page = requested_page - 1;
    if (requested_page < 0) {
        requested_page = 0;
        Serial.printf("Can't decrease page nymber ( is %d)\n",requested_page);
    } else {
        Serial.printf("Increasing page number to : %d\n",requested_page);
    };

 }

 static void print_flags(lv_obj_t * obj) {
    Serial.print("flags: ");
    if (lv_obj_has_flag(obj, LV_OBJ_FLAG_HIDDEN))         Serial.print("HIDDEN | ");
    if (lv_obj_has_flag(obj, LV_OBJ_FLAG_CLICKABLE))      Serial.print("CLICKABLE | ");
    if (lv_obj_has_flag(obj, LV_OBJ_FLAG_CHECKABLE))      Serial.print("CHECKABLE | ");
    if (lv_obj_has_flag(obj, LV_OBJ_FLAG_SCROLLABLE))     Serial.print("SCROLLABLE | ");
    if (lv_obj_has_flag(obj, LV_OBJ_FLAG_PRESS_LOCK))     Serial.print("PRESS_LOCK | ");
    if (lv_obj_has_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE))   Serial.print("EVENT_BUBBLE | ");
    if (lv_obj_has_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE)) Serial.print("GESTURE_BUBBLE | ");
    if (lv_obj_has_flag(obj, LV_OBJ_FLAG_EVENT_TRICKLE))   Serial.print("EVENT_TRICLE | ");
    if (lv_obj_has_flag(obj, LV_OBJ_FLAG_STATE_TRICKLE))   Serial.print("EVENT_TRICLE | ");
    Serial.println();

 }

static void event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * target_object = lv_event_get_target_obj(e);
    //lv_obj_flag_t raw_flags_value = target_object->flags ; 
    //uint32_t raw_flags_value = lv_obj_get_flags(target_object);
    
    //Serial.printf("(2) -  event name : %s\n", lv_event_code_get_name(code));
    //print_flags(target_object);

/*
    if(code == LV_EVENT_CLICKED) {
        //LV_LOG_USER("Clicked");
        Serial.println("Clicked");  
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {
        //LV_LOG_USER("Toggled");
        Serial.println("Toggled");
    }
*/
}

static void gesture_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);  
    Serial.print("event name : ");
    Serial.println(lv_event_code_get_name(code));

    if ( code == LV_EVENT_RELEASED || code == LV_EVENT_PRESS_LOST) {
        gesture_guard.gesture_handled = false; 
        Serial.println("Gesture: Ended");
    }
    else if(code == LV_EVENT_GESTURE) {
        lv_dir_t  dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        switch(dir) {
            case LV_DIR_TOP:
                Serial.println("Gesture: Top");
                break;
            case LV_DIR_BOTTOM:
                Serial.println("Gesture: Bottom");
                break;
            case LV_DIR_LEFT:
                //lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                //lv_obj_clear_flag(panel_main, LV_OBJ_FLAG_HIDDEN);
                //lv_obj_clear_state(panel_main, LV_STATE_DISABLED);
                if (! gesture_guard.gesture_handled ) { 
                    previous_page();
                    gesture_guard.gesture_handled = true;
                }
                Serial.println("Gesture: Left");
                break;

            case LV_DIR_RIGHT: 
                //lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x00FF00), LV_PART_MAIN | LV_STATE_DEFAULT);
                //lv_obj_add_flag(panel_main, LV_OBJ_FLAG_HIDDEN);
                //lv_obj_add_state(panel_main, LV_STATE_DISABLED);
                if (! gesture_guard.gesture_handled ) { 
                    next_page();
                    gesture_guard.gesture_handled = true;
                }
                Serial.println("Gesture: Right");
                break;
            default:
                Serial.println("Gesture: None");
                break;
        }
    }

}

static lv_obj_t * overlay = NULL;

void overlay_show_gesture_only(void) {
    lv_obj_remove_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
    
    overlay = lv_obj_create(lv_layer_top());
    lv_obj_set_size(overlay, LV_PCT(100), LV_PCT(100));
    lv_obj_align(overlay, LV_ALIGN_CENTER,0,0);
    lv_obj_add_flag(overlay,LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(overlay,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(overlay, LV_OBJ_FLAG_SCROLLABLE);

    // tarnbspartent, non clickable
    lv_obj_set_style_bg_opa(overlay,LV_OPA_TRANSP,0);
    
    //lv_obj_add_event_cb(overlay, gesture_event_cb, LV_EVENT_ALL,NULL);

    //lv_obj_add_event_cb(overlay, gesture_event_cb, LV_EVENT_GESTURE,NULL);
    //lv_obj_add_event_cb(overlay, gesture_event_cb, LV_EVENT_PRESS_LOST, NULL);
    //lv_obj_add_event_cb(overlay, gesture_event_cb, LV_EVENT_RELEASED, NULL);
    //lv_obj_add_event_cb(overlay, gesture_event_cb, LV_EVENT_PRESSED, NULL);
    //lv_obj_add_event_cb(overlay, gesture_event_cb, LV_EVENT_CLICKED, NULL); 

    //lv_obj_add_event_cb(lv_layer_top(),event_handler,LV_EVENT_ALL,NULL);
    //lv_obj_add_flag(lv_layer_top(),LV_OBJ_FLAG_EVENT_BUBBLE);
    //lv_obj_remove_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
    //Serial.println("top layer flags:");
    //print_flags(lv_layer_top());
    //Serial.println("-------------");
}
    


void init_test_screen( lv_obj_t * parent)
{
/* */
    int parent_width = lv_obj_get_width(parent);
    int parent_height = lv_obj_get_height(parent);
    int pageing_panel_height = 50;
    int container_height = parent_height - pageing_panel_height -10;

    // clear content of parent
    lv_obj_clean(parent);

    // Create conatainers
    lv_obj_t * panel_main = homey_panel(parent, parent_width, container_height, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_t * panel_pageing = homey_panel(parent, parent_width, pageing_panel_height, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    

    // Add buttons

    /*
    lv_obj_t * btn1 = homey_light_switch(container, "Button", DEFAULT_LV_OBJ_FLAG , 100);
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn1, LV_ALIGN_CENTER, -110, -110);
    lv_obj_remove_flag(btn1, LV_OBJ_FLAG_PRESS_LOCK);

    lv_obj_t * btn2 = homey_light_switch(container, "Button", DEFAULT_LV_OBJ_FLAG , 150);
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn2, LV_ALIGN_CENTER, -110, +110);
    lv_obj_remove_flag(btn2, LV_OBJ_FLAG_PRESS_LOCK);

    lv_obj_t * btn3 = homey_light_switch(container, "Button", DEFAULT_LV_OBJ_FLAG ,200);
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn3, LV_ALIGN_CENTER, +110, -110);
    lv_obj_remove_flag(btn3, LV_OBJ_FLAG_PRESS_LOCK);

    lv_obj_t * btn4 = homey_light_switch(container, "Toggle", LV_OBJ_FLAG_CHECKABLE,200);
    lv_obj_add_event_cb(btn4, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn4, LV_ALIGN_CENTER, +110, +110);
  */


    lv_obj_t * btn1 = homey_light_switch(panel_main, "Toggle", LV_OBJ_FLAG_CHECKABLE, 350,350);
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 0);


    //swipe
    //overlay_show_gesture_only();
    
    //lv_obj_add_event_cb(parent, gesture_event_cb, LV_EVENT_GESTURE, NULL);
    //lv_obj_add_event_cb(parent, gesture_event_cb, LV_EVENT_PRESS_LOST, NULL);
    //lv_obj_add_event_cb(parent, gesture_event_cb, LV_EVENT_RELEASED, NULL);

}

AsyncWebServer server(80);
bool ledOn = false;


void setup()
{
    Serial.begin(115200);

    my_lv_and_driver_init();
    Serial.println("LVGL initialized");
    // Create display content
    homey_styles_init();
    init_test_screen(lv_screen_active());

    Serial.println("Connecting to WiFi...");
    // Your WiFi connection code here   
    WiFi.mode(WIFI_STA);
    WiFi.begin("Telia-6589FB", "GAFfWyHvwpZJHN7R");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
    Serial.print("Werbserver running at : ")   ;
    Serial.println(WiFi.localIP());

    // create rest server

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/htnl", "ESP Asynv Server is running");
    });

    server.on("/led/on", HTTP_GET, [](AsyncWebServerRequest *request){
        ledOn = true;
        lv_obj_t * scr = lv_screen_active();
        //lv_obj_t * scr = lv_obj_create(NULL);
        // lv_screen_load(scr);
        lv_disp_set_theme(lv_obj_get_disp(scr), NULL);
        lv_obj_remove_style_all(scr);

        // Ensure any background image is cleared
        lv_obj_set_style_bg_image_src(scr,NULL,LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_dir(scr, LV_GRAD_DIR_NONE, LV_PART_MAIN);

        // Set a solid background color and full opacity
        lv_obj_set_style_bg_color(scr, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

        // If something full-screen is covering the screen, remove children (testing only)
        //lv_obj_clean(scr); // uncomment to test — this deletes all children (your buttons)

        // Force redraw
        lv_obj_invalidate(scr);
        lv_refr_now(lv_obj_get_disp(scr));
        Serial.println("LED is ON");


        request->send(200, "text/plain", "LED is ON");
    });

    server.on("/led/off", HTTP_GET, [](AsyncWebServerRequest *request){
        ledOn = false;
        // Set a solid background color and full opacity
        lv_obj_t * scr = lv_screen_active();
        lv_obj_set_style_bg_color(scr, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        request->send(200, "text/plain", "LED is OFF");
    });

    server.on("/led/status", HTTP_GET, [](AsyncWebServerRequest *request){
        if (ledOn)
            request->send(200, "text/plain", "LED is ON");
        else
            request->send(200, "text/plain", "LED is OFF");
    });

    server.begin();
    Serial.println("Setup done");

}

void loop()
{
    if (requested_page != current_page) {
        Serial.printf("Switching from page %d to %d\n", current_page, requested_page);
        // For testing, just change background color
        lv_obj_t * scr = lv_screen_active();
        if (requested_page == 0) {
            init_test_screen(scr);
            lv_obj_set_style_bg_color(scr, lv_color_hex(0x0000FF), LV_PART_MAIN | LV_STATE_DEFAULT);
        } else if (requested_page == 1) {
            init_test_screen(scr);
            lv_obj_set_style_bg_color(scr, lv_color_hex(0x00FF00), LV_PART_MAIN | LV_STATE_DEFAULT);
        } else {
            init_test_screen(scr);
            lv_obj_set_style_bg_color(scr, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
        }

        current_page = requested_page;
    }

  lv_timer_handler(); /* let the GUI do its work */
  lv_tick_inc(5);
  delay(5);
}
