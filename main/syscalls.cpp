#include "syscalls.h"
#include "globals.h"
#include "input.h"
#include "framebuffer.h"

void init_syscalls(WRState* wr_state) {
	// TODO: bind all syscalls here.
	wr_registerFunction(wr_state, "clearTerminal", 	_sys_clear_scr		);
    wr_registerFunction(wr_state, "print", 			_sys_print			);
	wr_registerFunction(wr_state, "printOut", 		_sys_printout		);
	wr_registerFunction(wr_state, "readInput", 		_sys_read_gm_input	);
	wr_registerFunction(wr_state, "isKeyDown", 		_sys_key_down		);
	wr_registerFunction(wr_state, "isKeyPressed", 	_sys_key_pressed	);
	wr_registerFunction(wr_state, "isKeyReleased", 	_sys_key_released	);
	wr_registerFunction(wr_state, "selectPressed", 	_sys_select_pressed	);
	wr_registerFunction(wr_state, "startPressed", 	_sys_start_pressed	); 
	// buffer calls
	wr_registerFunction(wr_state, "drawPixel", 	_sys_fb_draw_pixel	);
	wr_registerFunction(wr_state, "drawRectangle", 	_sys_fb_draw_rectangle	);
	wr_registerFunction(wr_state, "pushTFT", 	_sys_fb_push_tft	);
}



// TODO: implement all syscalls here.  
void _sys_fb_draw_pixel( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr ){
	if (argn < 3){
		return;
	}

	int x = argv[0].asInt();
	int y = argv[1].asInt();
	uint16_t color = (uint16_t)argv[2].asInt();

	fb_drawPixel(x, y, color);
}

void _sys_fb_draw_rectangle( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr ){
	if(argn < 5){
		return;
	}
	int x = argv[0].asInt();
	int y = argv[1].asInt();
	int width = argv[2].asInt();
	int height = argv[3].asInt();
	uint16_t color = (uint16_t)argv[4].asInt();

	fb_drawRectangle(x,y,width,height,color);

}

void _sys_fb_push_tft( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr ){
	fb_push_tft();
}

void _sys_clear_scr ( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr ) {
	tft.fillRect(0, 0, 320, 240, TFT_BLACK);
	tft.setCursor(0, 0);
}

void _sys_print ( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr ) {
	char buf[256];
	for( int i=0; i<argn; ++i ) {
		tft.printf( "%s", argv[i].asString(buf,1024) );
	}
}

void _sys_printout ( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr ) {
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

void _sys_select_pressed ( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr )
{ wr_makeInt(&retVal, select_pressed()); }

void _sys_start_pressed ( WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr )
{ wr_makeInt(&retVal, start_pressed()); }