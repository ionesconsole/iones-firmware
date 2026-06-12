#pragma once

#include <TFT_eSPI.h>
#include "SD.h"
#include "SPI.h"

extern TFT_eSPI tft;
extern TFT_eSprite frame_buffer_gui;
extern SPIClass sdspi;
extern char (*appnames)[9];
extern int appcount;