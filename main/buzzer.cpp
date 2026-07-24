#include "config.h"
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


void unload_sound_sheet(size_t id) {
    
    while(playing) vTaskDelay(1);

    if(id < 0 || id >= sound_sheets.size() || sound_sheets[id] == nullptr) return;
    
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
    printf("PSRAM Empty Space: %ld\n", ESP.getFreePsram());
}

TaskHandle_t buzzer_task;
const int* current_sound = nullptr;

void init_buzzer_task() {
    xTaskCreate(buzzer_loop, "buzzer", 2048, xTaskGetCurrentTaskHandle(), 2, &buzzer_task);
    
    // Wait until buzzer task is ready
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    printf("Buzzer task created.\n");
}

void buzzer_loop(void* arg) {
    TaskHandle_t creator = (TaskHandle_t)arg;

    size_t i ;
    int notedur;
    const int* sound;

    xTaskNotifyGive(creator); // Signal main task so that it can move on

    while(true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        
        sound = current_sound;
        
        i = 1;
        notedur = sound[0];
        playing = true;


        while(sound[i] != -1) {
            tone(BUZZER, sound[i++], notedur);

            // Proceed to the new sound if received new call
            if(ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(notedur))) {
                sound = current_sound;
                i = 1;
                notedur = sound[0];
                playing = true;
            }
        }

        playing = false;
    }
}

void play_sheet(size_t id) {

    if(id < 0 || id >= sound_sheets.size() || sound_sheets[id] == nullptr) return;
    current_sound = sound_sheets[id];
    xTaskNotifyGive(buzzer_task);
}