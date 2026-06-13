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

/**
 * Initializes the application list.
 * @important Don't forget to free the list after the desired value is selected.
 */
void init_list();

/**
 * Main loop that drives the GUI
 */
void gui_loop();

void gui_err();