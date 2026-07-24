#pragma once
#include <vector>
#include <cstdint>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * Initializes the buzzer task.
 */
void init_buzzer_task();

/**
 * Loads the sound sheet given from the application.
 * @param sound_sheet Integer array containing the note length and notes.
 * @returns ID to access the sound sheet.
 */
size_t load_sound_sheet(const int* sound_sheet);

/**
 * Unloads the sound sheet.
 * @param id ID of the sound to be unloaded.
 */
void unload_sound_sheet(size_t id);

/**
 * Prints the general information of the sound sheets that are currently loaded.
 */
void print_all_sheets();

/**
 * Unloads all the sound sheets loaded.
 */
void flush_sheets();

/**
 * Main function to run in buzzer task.
 */
void buzzer_loop(void* arg);

/**
 * Plays the sound sheet with the id given.
 * @param id The ID of the sound to be played.
 */
void play_sheet(size_t id);
