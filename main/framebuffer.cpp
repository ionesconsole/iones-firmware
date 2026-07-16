#include "framebuffer.h"
#include "globals.h"
#include "esp_heap_caps.h"

//static uint16_t* framebuffer = nullptr;
const int len = TFT_WIDTH * TFT_HEIGHT;

//FBDirtyRect dr;
//DirtyList dl;
FrameBuffer fb;
Viewport vp;


// fb_init(FB_DEPTH_BUFFER | FB_STENCIL_BUFFER | FB_DMA_BUFFER); example usage of flags
bool fb_init(uint8_t flags){
    
    size_t bytes = fb.height*fb.width*sizeof(uint16_t);
    fb.colorBuffer = (uint16_t*)ps_malloc(bytes);
    if(!fb.colorBuffer){
        Serial.printf("colorBuffer could not be allocated on psram..: %u bytes", bytes);
        
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
        fb.stencilBuffer = (uint8_t*)ps_malloc(bytes/2);
        if(!fb.stencilBuffer){
            Serial.printf("stencilBuffer could not be allocated on psram..: %u bytes", bytes);
            //fb_shutdown
            return false;
        }
        fb.hasStencil = true;
        Serial.printf("stencilBuffer allocated! -> %u bytes", bytes);
    }

    if (flags & FB_DMA)
    {
        #ifdef USE_DMA_TO_TFT
            tft.initDMA();
            //check for fb_init_dma
        #else
            Serial.printf("Could not initialize DMA...");
        #endif
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
    if(!fb.colorBuffer){ Serial.printf("No colorBuffer exists!");return;}

    for(int i = 0; i < len; i++){
        fb.colorBuffer[i] = color;
    } 
}
void fb_clear_depth(uint16_t depth){
    if(!fb.depthBuffer){ Serial.printf("No depthBuffer exists!");return;}

    for(int i = 0; i < len; i++){
        fb.depthBuffer[i] = depth;
    }  
}
void fb_clear_stencil(uint8_t stencil){
    if(!fb.stencilBuffer){ Serial.printf("No stencilBuffer exists!");return;}

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
    fb_shutdown_dma();
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
    if(!fb.colorBuffer){ Serial.printf("No framebuffer exists!"); return;}

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
    return fb.colorBuffer[x + fb.width*y];
}

void fb_drawRectangle(int x, int y, int width, int height, uint16_t color){
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
            fb.colorBuffer[j + i*fb.width] = color;            
        }
        
    }
}

void fb_drawPixel(int x, int y, uint16_t color) {
    if (!fb.colorBuffer) return;

    if (x < 0 || x >= fb.width) return;
    if (y < 0 || y >= fb.height) return;

    fb.colorBuffer[x + y * fb.width] = color;
}

void fb_print_info() {
    Serial.printf("Framebuffer: %dx%d\n", fb.width, fb.height);
    Serial.printf("Color:   %s\n", fb.colorBuffer ? "yes" : "no");
    Serial.printf("Depth:   %s\n", fb.depthBuffer ? "yes" : "no");
    Serial.printf("Stencil: %s\n", fb.stencilBuffer ? "yes" : "no");
}



void fb_push_tft(){
    if(!fb.colorBuffer){return;}
    tft.startWrite();
    tft.pushImage(0,0,fb.width,fb.height,fb.colorBuffer);
    tft.endWrite();

}

bool fb_init_dma(int lines){

    if (lines <= 0)
    {
        lines = 1;
    }

    if (lines > fb.height)
    {
        lines = fb.height;
    }

    if (!fb.colorBuffer)
    {
        Serial.printf("Cannot see color buffer!");
        return;
    }

    if (fb.dmaLineBuffer)
    {
        free(fb.dmaLineBuffer);
        fb.dmaLineBuffer = nullptr;
    }

    fb.dmaLineBuffer = ( uint16_t*)heap_caps_malloc(fb.width * lines * sizeof(uint16_t), MALLOC_CAP_DMA);
    if (!fb.dmaLineBuffer)
    {
        Serial.printf("DMA Init failed...");
        return;
    }
    else{
        Serial.printf("DMA line buffer allocated: %u bytes (%d lines)\n", bytes ,lines);
        return true;
    }
       
}

void fb_shutdown_dma(){
    if (fb.dmaLineBuffer) {
        free(fb.dmaLineBuffer);
        fb.dmaLineBuffer = nullptr;
    }

    fb.dmaEnabled = false;
    fb.dmaLineBufferHeight = 0;
}


void fb_push_tft(bool useDMA) {
    if (!fb.colorBuffer) {
        return;
    }

    if (useDMA) {
#ifdef USE_DMA_TO_TFT
        if (fb.dmaEnabled && fb.dmaLineBuffer && fb.dmaLineBufferHeight > 0) {
            int lines = fb.dmaLineBufferHeight;

            tft.startWrite();

            for (int y = 0; y < fb.height; y += lines) {
                int blockH = lines;

                if (y + blockH > fb.height) {
                    blockH = fb.height - y;
                }

                for (int row = 0; row < blockH; row++) {
                    uint16_t* src = fb.colorBuffer + (y + row) * fb.width;
                    uint16_t* dst = fb.dmaLineBuffer + row * fb.width;

                    memcpy(dst, src, fb.width * sizeof(uint16_t));
                }

                while (tft.dmaBusy()) {
                    delay(0);
                }

                tft.pushImageDMA(
                    0,
                    y,
                    fb.width,
                    blockH,
                    fb.dmaLineBuffer
                );
            }

            while (tft.dmaBusy()) {
                delay(0);
            }

            tft.endWrite();
            return;
        }
#endif
        // DMA istendi ama USE_DMA_TO_TFT kapalıysa veya buffer hazır değilse
        // alttaki normal pushImage yoluna düşer.
    }

    tft.startWrite();
    tft.pushImage(0, 0, fb.width, fb.height, fb.colorBuffer);
    tft.endWrite();
}
