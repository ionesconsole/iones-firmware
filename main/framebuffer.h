#pragma once
#include "globals.h"
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


#define FB_DEPTH_BUFFER   0x01
#define FB_STENCIL_BUFFER 0x02


struct DirtyRect{


};


struct FrameBuffer{
    int width;
    int height;
    uint16_t* colorBuffer;   
    uint16_t* depthBuffer;   // optional depth buffer
    uint8_t*  stencilBuffer; // optional stencil buffer
    bool hasColor;
    bool hasDepth;
    bool hasStencil;
    FrameBuffer()
        : width(TFT_WIDTH),
          height(TFT_HEIGHT),
          colorBuffer(nullptr),
          depthBuffer(nullptr),
          stencilBuffer(nullptr),
          hasColor(false),
          hasDepth(false),
          hasStencil(false)
    {
    }
};




bool fb_init(uint8_t flags = 0);

void fb_begin_frame(uint16_t color);
void fb_end_frame();

void fb_clear_color(uint16_t color = TFT_BLACK);
void fb_clear_depth(uint16_t depth = 0xFFFF);
void fb_clear_stencil(uint8_t stencil = 0);
void fb_clear_all(
    uint16_t color = TFT_BLACK,
    uint16_t depth = 0xFFFF,
    uint8_t stencil = 0
);
void fb_set_pixel(int x, int y, uint16_t color);
void fb_get_pixel();

//drawing functions

void fb_drawRectangle(int x, int y, int height, int width, uint16_t color);

//flush to tft
//void fb_push_wrapper();
void fb_push_tft();