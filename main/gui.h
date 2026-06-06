#pragma once

/**
 * Draws the splash screen.
 */
void splash();

/**
 * Draws the base of the UI
 */
void draw_ui_base(bool push = false);

/**
 * Shows the SD card error
 */
void draw_sd_error();

/**
 * Pushes the frame buffer to screen
 */
void push_frame();