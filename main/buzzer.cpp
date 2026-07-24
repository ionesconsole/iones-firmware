#include "buzzer.h"
#include "esp_heap_caps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

std::vector <const int*> sound_sheets;

bool playing = false;

size_t load_sound_sheet(const int* sound_sheet) {
    
    while(playing) vTaskDelay(1);

    size_t size = sound_sheets.size();
    
    // Find first available space and use there if available
    for(size_t i = 0; i < size; i++) {
        if(sound_sheets[i] == nullptr) {
            sound_sheets[i] = sound_sheet;
            printf("Found empty index at %d\n", i);
            return i;
        }
    }

    // Out of the loop -- append new sound and return the last index
    sound_sheets.push_back(sound_sheet);
    printf("No empty space found -- new item appended at %d\n", sound_sheets.size() - 1);
    return sound_sheets.size() - 1;
}


void unload_sound_sheet(int id) {
    
    while(playing) vTaskDelay(1);

    if(id <= 0 || id >= sound_sheets.size() || sound_sheets[id] == nullptr) return;
    
    heap_caps_free(const_cast<int*> (sound_sheets[id]));
    sound_sheets[id] = nullptr;

}

void flush_sheets() {
    while(playing) vTaskDelay(1);
    size_t size = sound_sheets.size();

    for(size_t i = 0; i < size; i++) heap_caps_free(const_cast<int*> (sound_sheets[i]));

    sound_sheets.clear();

    printf("Sound sheets are flushed.\n");
}


#include "Arduino.h"
void print_all_sheets() {

    size_t size = sound_sheets.size();

    printf("SIZE: %d\n", size);

    for(size_t i = 0; i < size; i++) {
        
        printf("%d \t", i);

        if(sound_sheets[i] == nullptr) {
            printf("NULL\n");
            continue;
        }

        printf("Speed: %d\tNotes:", sound_sheets[i][0]);

        for(int j = 1; sound_sheets[i][j] != -1; j++) printf("%d ", sound_sheets[i][j]);

        printf("\n");     
    }

    printf("--END--\n");
    printf("PSIRAM Empty Space: %ld\n", ESP.getFreePsram());
}