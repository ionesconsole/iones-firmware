#include "syscalls.h"
#include "globals.h"


void init_syscalls(WRState* wr_state) {
    wr_registerFunction(wr_state, "print", _sys_print);


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