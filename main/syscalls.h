#pragma once

#include "wrench.h"

/**
 * Binds the syscalls to the wrench script.
 */
void init_syscalls(WRState* wr_state);


// TODO: Declare all syscalls here.

/**
 * Clears screen terminal (Warning, the screen will go black when called);
 */
void _sys_clear_scr ( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr );

/**
 * Prints message to the screen.
 */
void _sys_print ( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr );

/**
 * Prints message to idf monitor.
 */
void _sys_printout ( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr );

/**
 * Reads the current state of the buttons.
 */
void _sys_read_gm_input ( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr );

/**
 * Checks if the key given is down.
 */
void _sys_key_down ( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr );

/**
 * Checks if the key given is pressed.
 */
void _sys_key_pressed ( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr );

/**
 * Checks if the key given is released.
 */
void _sys_key_released ( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr );

/**
 * Checks if Select was pressed.
 */
void _sys_select_pressed ( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr );

/**
 * Checks if Start was pressed.
 */
void _sys_start_pressed ( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr );