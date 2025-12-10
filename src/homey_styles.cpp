#include <lvgl.h>


lv_style_t homey_btn_style;
lv_style_t homey_panel_style;

//
static lv_color_t darken(const lv_color_filter_dsc_t * dsc, lv_color_t color, lv_opa_t opa)
{
    LV_UNUSED(dsc);
    //return lv_color_darken(color, opa);
    return lv_color_make(0,255,0);
}

void homey_styles_init(void)
{
        // Style for panels
    lv_style_init(&homey_panel_style);
    lv_style_set_bg_color(&homey_panel_style, lv_palette_main(LV_PALETTE_LIGHT_BLUE));
    lv_style_set_bg_opa(&homey_panel_style, LV_OPA_COVER);


    // Create a style for buttons   
    lv_style_init(&homey_btn_style);
    lv_style_set_bg_color(&homey_btn_style, lv_palette_main(LV_PALETTE_ORANGE));
    lv_style_set_bg_opa(&homey_btn_style, LV_OPA_COVER);


    /*
    // Create a simple button style
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


    //Create a style for the pressed state.
    //  Use a color filter to simply modify all colors in this state
    static lv_color_filter_dsc_t color_filter;
    lv_color_filter_dsc_init(&color_filter, darken);
    lv_style_init(&style_button_pressed);
    lv_style_set_color_filter_dsc(&style_button_pressed, &color_filter);
    lv_style_set_color_filter_opa(&style_button_pressed, LV_OPA_20);
    
    //*Create a red style. Change only some colors
    lv_style_init(&style_button_red);
    lv_style_set_bg_color(&style_button_red, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_bg_grad_color(&style_button_red, lv_palette_lighten(LV_PALETTE_RED, 3));
    
    lv_style_init(&style_def);
    lv_style_set_text_color(&style_def, lv_color_white());
    lv_style_set_transition(&style_def, &tr);
    */
}