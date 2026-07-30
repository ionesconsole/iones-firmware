#include <stdio.h>
#include <TFT_eSPI.h> 

#include "Arduino.h"
#include "gui.h"
#include "config.h"
#include "input.h"

#include "SD.h"
#include "SPI.h"
#include "globals.h"
#include "application.h"
#include "framebuffer.h"

// Init TFT interface
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite frame_buffer_gui = TFT_eSprite(&tft);
SPIClass sdspi(FSPI);





void io_init() {

    gpio_reset_pin(GM_CP);  // out
    gpio_reset_pin(GM_PL);  // out
    gpio_reset_pin(GM_Q7);  // in
    gpio_reset_pin(GM_SEL); // in
    gpio_reset_pin(GM_STR); // in

    gpio_set_direction(GM_CP,   GPIO_MODE_OUTPUT);
    gpio_set_direction(GM_PL,   GPIO_MODE_OUTPUT);
    gpio_set_direction(GM_Q7,   GPIO_MODE_INPUT);
    gpio_set_direction(GM_SEL,  GPIO_MODE_INPUT);
    gpio_set_direction(GM_STR,  GPIO_MODE_INPUT);

    gpio_set_pull_mode(GM_Q7,   GPIO_PULLDOWN_ONLY);
    gpio_set_pull_mode(GM_SEL,  GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GM_STR,  GPIO_PULLUP_ONLY);

}

void read_gm_input() {
    
    gpio_set_level(GM_PL, 0);
    gpio_set_level(GM_CP, 0);
    gpio_set_level(GM_CP, 1);
    gpio_set_level(GM_PL, 1);

    last_gm_inputs = gm_inputs;

    for(int i = 0; i < 8; i++) {
        gm_inputs <<= 1;
        gm_inputs |= gpio_get_level(GM_Q7);

        gpio_set_level(GM_CP, 0);
        gpio_set_level(GM_CP, 1);   
    }
}

// TODO: gm_select and gm_start are declared globally just in case.
// TODO: if in_place turns out to slow down the system, remove "bool" decleration to
// TODO: use the global ones.
bool select_pressed() {
    bool gm_select   = gpio_get_level(GM_SEL);
    bool selstartout = !gm_select && last_select;
    last_select = gm_select;
    return selstartout;
}

bool start_pressed() {
    bool gm_start   = gpio_get_level(GM_STR);
    bool selstartout = !gm_start && last_start;
    last_start = gm_start;
    return selstartout;
}


void test_native(){
    fb_init();
    fb_drawPixel(0,0,10);
    int x = fb_get_pixel(0,0);
    printf("pixel cords..: %d ", x);
    
}

extern "C" void app_main(void)
{
    // Initialise Arduino interface
    initArduino();

    // Initialise TFT display
    tft.init();
    tft.setRotation(3);
    tft.setSwapBytes(true);
    tft.setAttribute(PSRAM_ENABLE, true);

    //test_native();

    frame_buffer_gui.setAttribute(PSRAM_ENABLE, true);

    // init framebuffer
    fb_init();

    frame_buffer_gui.createSprite(320, 240);
    frame_buffer_gui.setSwapBytes(true);

    splash();

    // Mount
    bool sd_ok = SD.begin(SD_CS,  tft.getSPIinstance());
    delay(1000);

    // Mount fail --> fatal
    if(!sd_ok) {
        draw_sd_error();
        return; 
    }

    /*if (!SD.exists("/APP1")) {
        if (SD.mkdir("/APP1")) {
            printf("APP1 created.\n");
        } else {
            printf("APP1 creation failed.\n");
        }
    }*/


    io_init();

    // System FSM
    while(current_state != EXEC) {
        switch(current_state) {
        case LIST_INIT:
            init_list();
            printf("FREE SPACE after init: %ld\n", ESP.getFreePsram());
            break;
        case FATAL:
            printf("ENTERED FATAL");
            gui_err();
            printf("AFTER GUI_ERR");
            return;
        case GUI_LOOP:
            gui_loop();
            break;
        case LOAD:
            printf("FREE SPACE: %ld\n", ESP.getFreePsram());
            load_app();
            break;
            default:
            break;
        }
    }


    exec_app();
    printf("FREE SPACE after exec: %ld\n", ESP.getFreePsram());
 
}




