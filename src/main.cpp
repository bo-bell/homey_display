#include <Arduino.h>
#include <LovyanGFX.h>
#include <lvgl.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
//#include <ESPAsyncTCP.h>
#include <ArduinoJson.h>
#include "Guition_480x480_driver.h"
#include "homey_widgets.h"


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
/* */
    lv_obj_t * btn1 = homey_light_switch(parent, "Button", DEFAULT_LV_OBJ_FLAG , 100);
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn1, LV_ALIGN_CENTER, -110, -110);
    lv_obj_remove_flag(btn1, LV_OBJ_FLAG_PRESS_LOCK);

    lv_obj_t * btn2 = homey_light_switch(parent, "Button", DEFAULT_LV_OBJ_FLAG , 150);
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn2, LV_ALIGN_CENTER, -110, +110);
    lv_obj_remove_flag(btn2, LV_OBJ_FLAG_PRESS_LOCK);

    lv_obj_t * btn3 = homey_light_switch(parent, "Button", DEFAULT_LV_OBJ_FLAG ,200);
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn3, LV_ALIGN_CENTER, +110, -110);
    lv_obj_remove_flag(btn3, LV_OBJ_FLAG_PRESS_LOCK);

    lv_obj_t * btn4 = homey_light_switch(parent, "Toggle", LV_OBJ_FLAG_CHECKABLE,200);
    lv_obj_add_event_cb(btn4, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn4, LV_ALIGN_CENTER, +110, +110);
/**/

/*
    lv_obj_t * btn1 = homey_light_switch(parent, "Toggle", LV_OBJ_FLAG_CHECKABLE, 400);
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 0);
*/

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
        lv_disp_set_theme(NULL);
        
        lv_obj_set_style_bg_image_src(scr,NULL,LV_PART_MAIN);
        lv_obj_set_style_bg_grad_dir(scr, LV_GRAD_DIR_NONE, LV_PART_MAIN);

        lv_obj_set_style_bg_color(scr, lv_color_hex(0xFF0000), LV_PART_MAIN);
        lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
        request->send(200, "text/plain", "LED is ON");
    });

    server.on("/led/off", HTTP_GET, [](AsyncWebServerRequest *request){
        ledOn = false;
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
  lv_timer_handler(); /* let the GUI do its work */
  lv_tick_inc(5);
  delay(5);
}
