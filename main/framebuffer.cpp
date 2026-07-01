#include "framebuffer.h"
#include "globals.h"

//static uint16_t* framebuffer = nullptr;
const int len = TFT_WIDTH * TFT_HEIGHT;

FrameBuffer fb;

bool fb_init(uint8_t flags = 0){
    
    size_t bytes = fb.height*fb.width*sizeof(uint16_t);
    fb.colorBuffer = (uint16_t*)ps_malloc(bytes);
    if(!fb.colorBuffer){
        Serial.printf("colorBuffer could not be allocated on psram..: %u bytes", bytes);
        //fb_shutdown
        return false;
    }

    fb.hasColor = true;
    Serial.printf("colorBuffer allocated! -> %u bytes", bytes);
    
    if(flags & FB_DEPTH_BUFFER){
        fb.depthBuffer = (uint16_t*)ps_malloc(bytes);
        if(!fb.depthBuffer){
            Serial.printf("depthBuffer could not be allocated on psram..: %u bytes", bytes);
            //fb_shutdown
            return false;
        }
        fb.hasDepth = true;
        Serial.printf("depthBuffer allocated! -> %u bytes", bytes);
    }

    if(flags & FB_STENCIL_BUFFER){
        fb.stencilBuffer = (uint8_t*)ps_malloc(bytes);
        if(!fb.stencilBuffer){
            Serial.printf("stencilBuffer could not be allocated on psram..: %u bytes", bytes);
            //fb_shutdown
            return false;
        }
        fb.hasStencil = true;
        Serial.printf("stencilBuffer allocated! -> %u bytes", bytes);
    }


    if (fb.hasColor) {
        //fb_clear_color(TFT_BLACK);
    }

    if (fb.hasDepth) {
        //fb_clear_depth(0xFFFF);
    }

    if (fb.hasStencil) {
        //fb_clear_stencil(0);
    }

    return true;
}   


void fb_clear(uint16_t color){
    if(!framebuffer){ Serial.printf("No framebuffer exists!");}

    for(int i = 0; i < len; i++){
        framebuffer[i] = color;
    }

    //maybe fb_push later
}


void fb_set_pixel(int x, int y, uint16_t color){
    if(!framebuffer){ Serial.printf("No framebuffer exists!");}
    //edge cases either make them able to go negative or not accept negative
    int index = x + y*TFT_WIDTH;
    framebuffer[index] = color;
}

uint16_t fb_get_pixel(int x, int y){
    if(!framebuffer){
        return TFT_BLACK;
    }
    if(x<0 || x >= TFT_WIDTH){
        return TFT_BLACK;
    }
    if(y<0 || y >= TFT_HEIGHT){
        return TFT_BLACK;
    }
    return framebuffer[y + TFT_WIDTH*x];
}

void fb_push_tft(){


};