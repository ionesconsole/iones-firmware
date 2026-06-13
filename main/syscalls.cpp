#include "syscalls.h"
#include "globals.h"


void init_syscalls(WRState* wr_state) {
    wr_registerFunction(wr_state, "print", _sys_print);
	wr_registerFunction(wr_state, "up_press", _sys_up_press_test);
	wr_registerFunction(wr_state, "dn_press", _sys_dn_press_test);
	wr_registerFunction(wr_state, "a_press", _sys_a_press_test);


    // TODO: bind all syscalls here.
}

void _sys_print( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr )
{
	char buf[256];
	for( int i=0; i<argn; ++i )
	{
		tft.printf( "%s", argv[i].asString(buf,1024) );
	}
}


bool up_press();
bool dn_press();
bool a_press();

void _sys_up_press_test( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr ) {
	retVal = up_press();
}
void _sys_dn_press_test( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr ) {
	retVal = dn_press();
}

void _sys_a_press_test( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr ) {
	retVal = a_press();
}