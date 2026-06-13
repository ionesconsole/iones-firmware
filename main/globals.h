#pragma once

#include <TFT_eSPI.h>
#include "SD.h"
#include "SPI.h"

extern TFT_eSPI tft;
extern TFT_eSprite frame_buffer_gui;
extern SPIClass sdspi;

extern char (*appnames)[9];
extern int appcount;

enum sys_state {
    LIST_INIT,
    GUI_LOOP,
    LOAD,
    EXEC,
    FATAL
};

extern enum sys_state current_state;
extern const char* current_err;
extern char selectedapp[9];