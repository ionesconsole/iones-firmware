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
 * Initializes input pins.
 */
void io_init();

/**
 * Reads the current state of the buttons.
 */
void read_gm_input();