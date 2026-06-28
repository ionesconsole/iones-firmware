#include "framebuffer.h"
#include "globals.h"

static uint16_t* framebuffer = nullptr;
const int len = TFT_WIDTH * TFT_HEIGHT;

bool fb_init(){
    
    size_t bytes = TFT_WIDTH * TFT_HEIGHT * sizeof(uint16_t);

    framebuffer = (uint16_t*)ps_malloc(bytes);

    if(!framebuffer){
        Serial.printf("Framebuffer could not be allocated on psram..: %u bytes", bytes);
    }

    Serial.printf("Framebuffer allocated! -> %u bytes", bytes);

    //fb_clear();
    return true;
}


void fb_clear(uint16_t color){
    if(!framebuffer){ Serial.printf("No framebuffer exists!");}

    for(int i = 0; i < len; i++){
        framebuffer[i] = color;
    }

    //maybe fb_push later
}


void fb_set_pix(int x, int y, uint16_t color){
    if(!framebuffer){ Serial.printf("No framebuffer exists!");}
    //edge cases either make them able to go negative or not accept negative
    int index = x + y*TFT_WIDTH;
    framebuffer[index] = color;
}

void fb_push_tft(){
    

};