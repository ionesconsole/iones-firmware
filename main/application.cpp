#include "application.h"
#include "wrench.h"
#include "SD.h"
#include "config.h"
#include "globals.h"
#include "gui.h"

#define WRENCH_WITHOUT_COMPILER

void load_app() {

    // Getting the full path of the execfile
    char execpath[PATH_MAX_IOWR];
    snprintf(execpath, PATH_MAX_IOWR, "/%s/%s", selectedapp, "execfile");
    printf("To be executed: %s\n", execpath);

    // Check existance of the execfile
    if(!SD.exists(execpath)) {
        printf( "No executable file found.\n");
        current_err = "No executable file found.";
        gui_err();
        delay(3000);
        current_state = LIST_INIT;
        return;
    }

    // Open the execfile -- abort if fails
    File execfile = SD.open(execpath);
    if(!execfile) {
        printf("The file could not be opened.\n");
        current_err = "The file could not be opened.";
        delay(3000);
        current_state = LIST_INIT;
        return;
    }

    // Get the size of the execfile
    exec_len_B = execfile.size();

    // Allocate memory for bytecode
    exec_bytes = (unsigned char*) heap_caps_malloc(exec_len_B, MALLOC_CAP_SPIRAM);

    // Abort if fails to allocate
    if(exec_bytes == nullptr) {
        execfile.close();
        printf("Memory could not be allocated for executable.\n");
        current_err = "Memory could not be allocated for executable.";
        delay(3000);
        current_state = LIST_INIT;
        return;
    }

    // Fetch the contents of the execile
    size_t readsize = execfile.read(exec_bytes, exec_len_B);

    execfile.close();

    // Something gone wrong if this is the case
    if(readsize != exec_len_B) {
        heap_caps_free(exec_bytes);
        printf("File could not be read properly -- PSRAM freed\n");
        current_err = "File could not be read properly -- PSRAM freed.";
        delay(3000);
        current_state = LIST_INIT;
        return;
    }

    printf("\nFILE READ SUCCESSFULLY\nSize: %d bytes.\n", readsize);
    current_state = EXEC;

    //! TEMP
    tft.fillRect(0, 0, 320, 240, TFT_BLACK);


}

#include "syscalls.h"
void exec_app() {

    // Initialize WRENCH VM and bind syscalls
    WRState* wr_state = wr_newState();
    init_syscalls(wr_state);

    // For debug
    printf("Before: %u\n", uxTaskGetStackHighWaterMark(NULL));

    // Run the application
    wr_run(wr_state, exec_bytes, exec_len_B);

    // For debug
    printf("After: %u\n", uxTaskGetStackHighWaterMark(NULL));

    // Cleanup
    heap_caps_free(exec_bytes);
    wr_destroyState(wr_state);
}