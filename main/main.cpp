#include <stdio.h>
#include <TFT_eSPI.h> 

#include "Arduino.h"
#include "gui.h"
#include "config.h"

#include "SD.h"
#include "SPI.h"
#include "globals.h"

// Init TFT interface
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite frame_buffer_gui = TFT_eSprite(&tft);
SPIClass sdspi(FSPI);




// _______________TEMP IO_______________ //
bool last_dn = false;
bool last_up = false;
bool last_a  = false;
#define UP GPIO_NUM_5
#define DN GPIO_NUM_6
#define GM_A GPIO_NUM_7

void io_init() {
    gpio_reset_pin(UP);
    gpio_set_pull_mode(UP, GPIO_PULLDOWN_ONLY);
    gpio_set_direction(UP, GPIO_MODE_INPUT);
    gpio_reset_pin(DN);
    gpio_set_pull_mode(DN, GPIO_PULLDOWN_ONLY);
    gpio_set_direction(DN, GPIO_MODE_INPUT);
    gpio_reset_pin(GM_A);
    gpio_set_pull_mode(GM_A, GPIO_PULLDOWN_ONLY);
    gpio_set_direction(GM_A, GPIO_MODE_INPUT);
}

bool up_press() {
    bool up = gpio_get_level(UP);
    bool ret = last_up == false && up;
    last_up = up;
    return ret;
}
bool dn_press() {
    bool dn = gpio_get_level(DN);
    bool ret = last_dn == false && dn;
    last_dn = dn;
    return ret;
}
bool a_press() {
    bool a = gpio_get_level(GM_A);
    bool ret = last_a == false && a;
    last_a = a;
    return ret;
}
// ____________END OF TEMP IO_____________ //

extern "C" void app_main(void)
{
    // Initialise Arduino interface
    initArduino();

    // Initialise TFT display
    tft.init();
    tft.setRotation(3);
    tft.setSwapBytes(true);
    tft.setAttribute(PSRAM_ENABLE, true);

    frame_buffer_gui.setAttribute(PSRAM_ENABLE, true);

    // init framebuffer
    frame_buffer_gui.createSprite(320, 240);
    frame_buffer_gui.setSwapBytes(true);

    splash();

    // Mount
    bool sd_ok = SD.begin(SD_CS,  tft.getSPIinstance());
    delay(1000);

    draw_ui_base();

    // Mount fail --> fatal
    if(!sd_ok) {
        draw_sd_error();
        push_frame();
        return;
    }


    io_init();
    while(current_state != EXEC) {
        switch(current_state) {
            case LIST_INIT:
            init_list();
            printf("FREE SPACE after init: %ld\n", ESP.getFreePsram());
            break;
            case FATAL:
            fatal_err();
            return;
            case GUI_LOOP:
            gui_loop();
            break;
            case LOAD:
            printf("Loading %s...\n", selectedapp);
            tft.fillRect(0, 0, 320, 240, TFT_BLUE);
            tft.setCursor(0, 0);
            tft.printf("Loading %s...", selectedapp);
            printf("FREE SPACE: %ld\n", ESP.getFreePsram());
            delay(1000);
            current_state = LIST_INIT;
            break;
            default:
            break;
        }
    }
 
}




