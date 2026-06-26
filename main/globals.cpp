#include "globals.h"
#include "config.h"

char (*appnames)[9];

int appcount = 0;

enum sys_state current_state = LIST_INIT;

const char* current_err;

char selectedapp[9];

unsigned char* exec_bytes = nullptr;

size_t exec_len_B;

uint8_t gm_inputs;
uint8_t last_gm_inputs;