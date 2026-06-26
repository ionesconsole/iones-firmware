#pragma once

#include "wrench.h"


/**
 * Binds the syscalls to the wrench script.
 */
void init_syscalls(WRState* wr_state);

/**
 * Prints message to the screen
 */
void _sys_print( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr );

// TODO: Declare all syscalls here.