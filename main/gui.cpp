#include <TFT_eSPI.h> 
#include "gui.h"
#include "globals.h"
#include "config.h"
#include "bitmaps.h"

void splash() {

    // init logo
    TFT_eSprite* logo_sprite = new TFT_eSprite(&tft);
    logo_sprite -> setAttribute(PSRAM_ENABLE, true);

    logo_sprite -> createSprite(187, 51);
    //logo_sprite -> createSprite(1000, 1000);
    logo_sprite -> pushImage(0, 0, 187, 51, logo);

    // draw splash screen
    frame_buffer.fillSprite(TFT_DARKGREY);
    logo_sprite -> pushToSprite(&frame_buffer, 67, 95, HATED_COLOR);
    frame_buffer.pushSprite(0, 0);

    if(frame_buffer.getAttribute(PSRAM_ENABLE))
        printf("FB PSRAM\n");
    if(logo_sprite -> getAttribute(PSRAM_ENABLE))
        printf("FB PSRAM\n");

    delay(100);

    delete logo_sprite;

}