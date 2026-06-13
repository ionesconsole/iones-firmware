#pragma once

#include "wrench.h"


void init_syscalls(WRState* wr_state);
void _sys_print( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr );
void _sys_up_press_test( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr );
void _sys_dn_press_test( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr );
void _sys_a_press_test( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr );