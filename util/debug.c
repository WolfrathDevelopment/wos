/*
 * debug.c
 * Wolfrath/Kriewall, 2013
 *
 * Implementation of debug functions
 */

#include "debug.h"

void _debug(char * msg){

	put_string("DEBUG> ");
	put_string(msg);
	put_char('\n');
}

void _panic(char * msg){

	put_string("\nPANIC> ");
	put_string(msg);

	for(;;)
		;
}

void _assert(uint exp){

	if(!exp)
		_panic("Assertion Failed");
}

