#include <stdio.h>
#include <TFT_eSPI.h> 

#include "Arduino.h"
#include "gui.h"
#include "config.h"

#include "SD.h"
#include "SPI.h"

// Init TFT interface
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite frame_buffer_gui = TFT_eSprite(&tft);
SPIClass sdspi(FSPI);
char (*appnames)[9];
int appcount = 0;


// _______________TEMP IO_______________ //
bool last_dn = false;
bool last_up = false;
#define UP GPIO_NUM_5
#define DN GPIO_NUM_6

void io_init() {
    gpio_reset_pin(UP);
    gpio_set_pull_mode(UP, GPIO_PULLDOWN_ONLY);
    gpio_set_direction(UP, GPIO_MODE_INPUT);
    gpio_reset_pin(DN);
    gpio_set_pull_mode(DN, GPIO_PULLDOWN_ONLY);
    gpio_set_direction(DN, GPIO_MODE_INPUT);
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

    bool sd_ok = SD.begin(SD_CS,  tft.getSPIinstance());
    delay(1000);

    draw_ui_base();

    if(!sd_ok) {
        draw_sd_error();
        push_frame();
        return;
    }


    io_init();
    init_list();
    gui_loop();
    // RUNGAME
 
}




