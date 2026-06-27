#pragma once
#include "config.h"

/**
 * Input taken from game buttons.
 */
extern uint8_t gm_inputs;

/**
 * The previous values of the game buttons.
 */
extern uint8_t last_gm_inputs;

/**
*  Input taken from Select button.
*/
extern bool    gm_select;

// TODO: uncomment gm_start, last_select, and selstartout if sel-start check turns out to slow games down
/**
*  Input taken from Start button.
*/
extern bool    gm_start;

/**
*  The previous value of the Select button.
*/
extern bool    last_select;

extern bool    selstartout;

/**
*  The previous value of the Start button.
*/
extern bool    last_start;

/**
 * Initializes input pins.
 */
void io_init();

/**
 * Reads the current state of the buttons.
 */
void read_gm_input();

/**
 * Checks if Select was pressed.
 */
bool select_pressed();

/**
 * Checks if Start was pressed.
 */
bool start_pressed();