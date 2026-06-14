#pragma once

/**
 * Draws the splash screen.
 * Pushed automatically.
 */
void splash();

/**
 * Draws the SD card error to the frame buffer.
 */
void draw_sd_error();

/**
 * Initializes the application list.
 * Don't forget to free the list after the desired value is selected.
 */
void init_list();

/**
 * Main loop that drives the GUI
 */
void gui_loop();

/**
 * Draws the system error messages.
 * Don't forget to assign current_err to the string desired before calling this function.
 * Pushed automatically.
 */
void gui_err();