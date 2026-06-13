#include "application.h"
#include "wrench.h"
#include "SD.h"
#include "config.h"
#include "globals.h"
#include "gui.h"

#define WRENCH_WITHOUT_COMPILER

void load_app() {

    char execpath[PATH_MAX_IOWR];
    snprintf(execpath, PATH_MAX_IOWR, "/%s/%s", selectedapp, "execfile");
    printf("To be executed: %s\n", execpath);

    if(!SD.exists(execpath)) {
        printf("EXEC file DNE\n");
        current_err = "The file does not exist.";
        gui_err();
        delay(3000);
        current_state = LIST_INIT;
        return;
    }

    File execfile = SD.open(execpath);
    if(!execfile) {
        printf("EXEC file could not be opened\n");
        current_err = "The file could not be opened.";
        delay(3000);
        current_state = LIST_INIT;
        return;
    }

    exec_len_B = execfile.size();

    exec_bytes = (unsigned char*) heap_caps_malloc(exec_len_B, MALLOC_CAP_SPIRAM);

    if(exec_bytes == nullptr) {
        execfile.close();
        printf("Memory could not be allocated for executable.\n");
        current_err = "Memory could not be allocated for executable.";
        delay(3000);
        current_state = LIST_INIT;
        return;
    }

    size_t readsize = execfile.read(exec_bytes, exec_len_B);

    execfile.close();

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

    WRState* wr_state = wr_newState();
    init_syscalls(wr_state);

    printf("Before: %u\n", uxTaskGetStackHighWaterMark(NULL));

    // Run the application
    wr_run(wr_state, exec_bytes, exec_len_B);

    printf("After: %u\n", uxTaskGetStackHighWaterMark(NULL));

    // Cleanup
    heap_caps_free(exec_bytes);
    wr_destroyState(wr_state);
}