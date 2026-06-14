#pragma once

#include <TFT_eSPI.h>
#include "SD.h"
#include "SPI.h"

extern TFT_eSPI tft;
extern TFT_eSprite frame_buffer_gui;
extern SPIClass sdspi;

extern char (*appnames)[9];

/**
 * Total number of apps available (folders in root)
 */
extern int appcount;

enum sys_state {
    LIST_INIT,
    GUI_LOOP,
    LOAD,
    EXEC,
    FATAL
};

/**
 * Current state of the OS.
 */
extern enum sys_state current_state;

/**
 * Message to be shown on Fatal Error state.
 */
extern const char* current_err;

/**
 * Currently loading/running application name.
 */
extern char selectedapp[9];

/**
 * Pointer to content of the executable bytecode.
 * Bytecode sould be stored in (P)SRAM.
 */
extern unsigned char* exec_bytes;

/**
 * Length of executable bytecode in Bytes.
 */
extern size_t exec_len_B;