#include <stdio.h>
#include <TFT_eSPI.h> 

#include "Arduino.h"
#include "gui.h"
#include "config.h"

#include "SD.h"
#include "SPI.h"

// Init TFT interface
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite frame_buffer = TFT_eSprite(&tft);
SPIClass sdspi(FSPI);


extern "C" void app_main(void)
{
    // Initialise Arduino interface
    initArduino();

    // Initialise TFT display
    tft.init();
    tft.setRotation(3);
    tft.setSwapBytes(true);
    tft.setAttribute(PSRAM_ENABLE, true);

    frame_buffer.setAttribute(PSRAM_ENABLE, true);

    // init framebuffer
    frame_buffer.createSprite(320, 240);
    frame_buffer.setSwapBytes(true);

    splash();

    if(SD.begin(SD_CS,  tft.getSPIinstance()))
        tft.printf("SD SUCCESS\n");
    else {
        tft.printf("SD FAIL\n");
    }

    delay(1000);
}





