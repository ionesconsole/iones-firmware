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


/**
 * TEMP: check up key
 */
void _sys_up_press_test( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr );

/**
 * TEMP: check dn key
 */
void _sys_dn_press_test( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr );

/**
 * TEMP: check a key
 */
void _sys_a_press_test( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr );