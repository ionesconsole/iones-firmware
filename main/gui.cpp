#include <TFT_eSPI.h> 
#include "gui.h"
#include "globals.h"
#include "config.h"
#include "bitmaps.h"

void splash() {
    frame_buffer.fillSprite(TFT_DARKGREY);
    frame_buffer.pushImage(67, 95, 187, 51, logo);
    frame_buffer.pushSprite(0, 0);
}


void draw_ui_base(bool push) {
    frame_buffer.fillSprite(TFT_WHITE);
    frame_buffer.fillRect(0, 220, 320, 20, TFT_LIGHTGREY);

    if(push) frame_buffer.pushSprite(0, 0);
}

void draw_sd_error() {

    frame_buffer.pushImage(139, 72, 42, 56, sd_error);
    frame_buffer.setTextColor(TFT_DARKGREY);
    frame_buffer.drawString("SD card could not be read.", 75, 140, 2);
    frame_buffer.drawString("Press RESET to try again.", 80, 160, 2);
}

// 21 28
// 320 240 --> 160 120 --> x: 139 y: 92

void push_frame() { frame_buffer.pushSprite(0, 0); }