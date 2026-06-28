#pragma once

#include <Arduino.h>
#include <stdint.h>

#define TFT_WIDTH  32
#define TFT_HEIGHT 16

#define RGB565_BLACK 0x0000
#define RGB565_WHITE 0xFFFF
#define RGB565_RED   0xF800
#define RGB565_GREEN 0x07E0
#define RGB565_BLUE  0x001F
#define RGB565_YELLOW 0xFFE0
#define RGB565_MAGENTA 0xF81F
#define RGB565_CYAN 0x07FF

bool fb_init();

void fb_begin_frame(uint16_t color);
void fb_end_frame();

void fb_clear(uint16_t color);
void fb_set_pix(int x, int y, uint16_t color);

//drawing functions

void fb_drawRectangle(int x, int y, int height, int weight, uint16_t color);

//flush to tft

void fb_push_tft();