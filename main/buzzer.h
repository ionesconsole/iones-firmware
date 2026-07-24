#pragma once
#include <vector>
#include <cstdint>


size_t load_sound_sheet(const int* sound_sheet);

void unload_sound_sheet(int id);

void print_all_sheets();

void flush_sheets();

