#include <TFT_eSPI.h> 
#include "gui.h"
#include "globals.h"
#include "config.h"
#include "bitmaps.h"

/**
 * Draws the outline of the UI to the frame buffer
 */
void draw_ui_base() {
    frame_buffer_gui.fillSprite(TFT_WHITE);
    frame_buffer_gui.fillRect(0, 220, 320, 20, TFT_LIGHTGREY);
    frame_buffer_gui.fillRectVGradient(0, 220, 320, 5, TFT_DARKGREY, TFT_LIGHTGREY);
}

/**
 * Pushes the frame buffer to the screen
 */
void push_frame() { frame_buffer_gui.pushSprite(0, 0); }

void splash() {
    frame_buffer_gui.fillSprite(TFT_DARKGREY);
    frame_buffer_gui.pushImage(67, 95, 187, 51, logo);
    push_frame();
}

void draw_sd_error() {
    draw_ui_base();
    frame_buffer_gui.pushImage(139, 72, 42, 56, sd_error);
    frame_buffer_gui.setTextColor(TFT_DARKGREY);
    frame_buffer_gui.drawString("SD card could not be read.", 75, 140, 2);
    frame_buffer_gui.drawString("Press RESET to try again.", 80, 160, 2);
    push_frame();
}

void init_list() {
    File root = SD.open("/");
    appcount = 0;
    
    // Count directories:
    while(true) {
        File entry = root.openNextFile();
        if(!entry) break;
        if(entry.isDirectory()) appcount++;
        entry.close();
    }

    if(appcount == 0) {
        root.close();
        current_state = FATAL;
        current_err = "No applications found.";
        //printf("No application found.\nClosed root.\nList was not allocated anyway.\n");
        return;
    }

    appnames = (char (*) [9]) heap_caps_malloc(appcount * sizeof(*appnames), MALLOC_CAP_SPIRAM);

    if(appnames == nullptr) {
        root.close();
        current_state = FATAL;
        current_err = "Somehow, the application list could not be initialized.";
        //printf("Something went wrong while allocating list.\nClosed root.\n");
        return;
    }
    
    root.rewindDirectory();
    for(int i = 0; i < appcount; i++) {
        File entry = root.openNextFile();
        if(!entry) break;
        if(!entry.isDirectory()) { i--; continue; }
        strncpy(appnames[i], entry.name(), 9);
        entry.close();
    }

    root.close();

    printf("List initialised successfully.\nClosed root.\nList remains allocated.\n");
    printf("App count: %d\n", appcount);
    //for(int i = 0; i < appcount; i++) printf("%s\n", appnames[i]);

    current_state = GUI_LOOP;

}

/**
 * Draws the outlines of the rendered list to the frame buffer
 */
void draw_list_base() {
    frame_buffer_gui.fillRect(0, 0, 320, 220, TFT_WHITE);
    frame_buffer_gui.fillRectVGradient(0, 88, 320, 44, TFT_SKYBLUE, TFT_BLUE);
    frame_buffer_gui.fillRectVGradient(0, 132, 320, 6, TFT_LIGHTGREY, TFT_WHITE);
    //for(int i = 0; i < 220; i += 44) frame_buffer_gui.drawLine(0, i, 319, i, TFT_LIGHTGREY);

}

/**
 * Draws the portion of the list to the frame buffer, according to the selected index 
 */
void render_list(int index = 0) {

    // Preparing the N of N text
    char locstr[23];
    snprintf(locstr, 23, "%d of %d", index + 1, appcount);
    draw_list_base();

    frame_buffer_gui.setTextColor(TFT_DARKGREY);
    if(index >= 2) frame_buffer_gui.drawString(appnames[index - 2], 20, 15, 2);
    if(index >= 1) frame_buffer_gui.drawString(appnames[index - 1], 20, 59, 2);

    frame_buffer_gui.setTextColor(TFT_WHITE);
    frame_buffer_gui.drawString(appnames[index], 20, 103, 2);
    frame_buffer_gui.drawRightString(locstr, 300, 103, 2);
    
    frame_buffer_gui.setTextColor(TFT_DARKGREY);
    if(index + 1 < appcount) frame_buffer_gui.drawString(appnames[index + 1], 20, 147, 2);
    if(index + 2 < appcount) frame_buffer_gui.drawString(appnames[index + 2], 20, 191, 2);


    push_frame();
}

#include "input.h"
void gui_loop() {

    draw_ui_base();

    frame_buffer_gui.setTextColor(TFT_BLACK);
    frame_buffer_gui.drawString ("A : Launch", 20, 223, 2);

    int current_app_index = 0;

    render_list();

    while(true) {

        read_gm_input();
        
        if(gm_inputs & GM_U && !(last_gm_inputs & GM_U) && current_app_index > 0) {
            current_app_index--;
            render_list(current_app_index);
        } else if(gm_inputs & GM_D && !(last_gm_inputs & GM_D) && current_app_index < appcount-1) {
            current_app_index++;
            render_list(current_app_index);
        } else if(gm_inputs & GM_A && !(last_gm_inputs & GM_A)) break;
    
        delay(10);

    }

    strncpy(selectedapp, appnames[current_app_index], 9);
    heap_caps_free(appnames); // !!! App list freed
    printf("App names freed.\n");

    current_state = LOAD;
    return;
}

void gui_err() {
    draw_ui_base();
    frame_buffer_gui.pushImage(20, 40, 40, 34, exclam);
    frame_buffer_gui.setTextColor(TFT_DARKGREY);
    frame_buffer_gui.drawString(current_err, 20, 100, 2);
    push_frame();
}