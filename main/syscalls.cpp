#include "syscalls.h"
#include "globals.h"
#include "input.h"


void init_syscalls(WRState* wr_state) {
	// TODO: bind all syscalls here.
    wr_registerFunction(wr_state, "print", 			_sys_print			);
	wr_registerFunction(wr_state, "printOut", 		_sys_printout		);
	wr_registerFunction(wr_state, "readInput", 		_sys_read_gm_input	);
	wr_registerFunction(wr_state, "isKeyDown", 		_sys_key_down		);
	wr_registerFunction(wr_state, "isKeyPressed", 	_sys_key_pressed	);
	wr_registerFunction(wr_state, "isKeyReleased", 	_sys_key_released	);
}



// TODO: implement all syscalls here.

void _sys_print( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr ) {
	char buf[256];
	for( int i=0; i<argn; ++i ) {
		tft.printf( "%s", argv[i].asString(buf,1024) );
	}
}

void _sys_printout( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr ) {
	char buf[256];
	for( int i=0; i<argn; ++i ) {
		printf( "%s", argv[i].asString(buf,1024) );
	}
}

void _sys_read_gm_input ( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr )
{ read_gm_input(); }

void _sys_key_down ( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr )
{ wr_makeInt(&retVal, gm_inputs & (uint8_t) argv -> asInt()); }

void _sys_key_pressed ( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr )
{ wr_makeInt(&retVal, gm_inputs & (uint8_t) argv -> asInt() && !(last_gm_inputs & (uint8_t) argv -> asInt())); }

void _sys_key_released ( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr )
{ wr_makeInt(&retVal, !(gm_inputs & (uint8_t) argv -> asInt()) && last_gm_inputs & (uint8_t) argv -> asInt()); }