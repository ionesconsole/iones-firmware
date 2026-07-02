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


void fb_clear_color(uint16_t color){
    if(!fb.colorBuffer){ Serial.printf("No colorBuffer exists!");}

    for(int i = 0; i < len; i++){
        fb.colorBuffer[i] = color;
    } 
}
void fb_clear_depth(uint16_t depth){
    if(!fb.depthBuffer){ Serial.printf("No depthBuffer exists!");}

    for(int i = 0; i < len; i++){
        fb.depthBuffer[i] = depth;
    }  
}
void fb_clear_stencil(uint8_t stencil){
    if(!fb.stencilBuffer){ Serial.printf("No stencilBuffer exists!");}

    for(int i = 0; i < len; i++){
        fb.stencilBuffer[i] = stencil;
    }
}
void fb_clear_all(uint16_t color, uint16_t depth, uint8_t stencil) {
    fb_clear_color(color);

    if (fb.depthBuffer) {
        fb_clear_depth(depth);
    }

    if (fb.stencilBuffer) {
        fb_clear_stencil(stencil);
    }
}

void fb_shutdown(){
    if(fb.colorBuffer){
        free(fb.colorBuffer);
        fb.colorBuffer = nullptr;
    }
    if(fb.depthBuffer){
        free(fb.depthBuffer);
        fb.depthBuffer = nullptr;
    }
    if(fb.stencilBuffer){
        free(fb.stencilBuffer);
        fb.stencilBuffer = nullptr;
    }

    fb.hasColor = false;
    fb.hasDepth = false;
    fb.hasStencil = false;
}


void fb_set_pixel(int x, int y, uint16_t color){
    if(!fb.colorBuffer){ Serial.printf("No framebuffer exists!");}

    if (x < 0 || x >= TFT_WIDTH)
    {
        Serial.printf("Out of bounds - width!");
        return;
    }
    if (y < 0 || y >= TFT_HEIGHT)
    {
        Serial.printf("Out of bounds - height!");
        return;
    }
    //edge cases either make them able to go negative or not accept negative
    int index = x + y*TFT_WIDTH;
    fb.colorBuffer[index] = color;
}

uint16_t fb_get_pixel(int x, int y){
    if(!fb.colorBuffer){
        return TFT_BLACK;
    }
    if(x<0 || x >= TFT_WIDTH){
        return TFT_BLACK;
    }
    if(y<0 || y >= TFT_HEIGHT){
        return TFT_BLACK;
    }
    return fb.colorBuffer[y + TFT_WIDTH*x];
}

void fb_drawRectangle(int x, int y, int height, int width, uint16_t color){
    if (x<0 || width > TFT_WIDTH)
    {
        return;
    }
    if (y<0 || height > TFT_HEIGHT)
    {
        return;
    }

    //edge case for color could exist

    for (int i = y; i < y + height; i++)
    {
        for (int j = x; j < x + width; j++)
        {
            fb.colorBuffer[x + y*fb.width] = color;            
        }
        
    }
    

}

void fb_push_tft(){
    if(!fb.colorBuffer){return;}
    tft.startWrite();
    tft.pushImage(0,0,fb.width,fb.height,fb.colorBuffer);
    tft.endWrite();

}