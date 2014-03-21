/*
 * debug.c
 * Joel Wolfrath, 2013
 *
 * Implementation of debug functions
 */

#include "debug.h"

void trace_stack(w_uint32 frames){

	w_uint32* ebp = &frames - 2;
	printf("Stack trace:\n");

	w_uint32 frame;

	for(frame = 0; frame < frames; ++frame){

		w_uint32 eip = ebp[1];

		/* Check to make sure there is a caller */
		if(eip == 0)
			break;

		/* Go back one more frame */

		ebp = (uint*)ebp[0];
		w_uint32* args = &ebp[2];
		printf("  0x%p\n", eip);
	}
}

void _debug(char* msg, char* f, w_uint32 ln){

	printf("\nDEBUG (%s at line %d)> %s\n", f, ln, msg);
}

void _panic(char* msg, char* f, w_uint32 ln){

	printf("\nPANIC (%s at line %d)> %s", f, ln, msg);
	for(;;)
		;
}

void _assert(w_int32 exp, char* f, w_uint32 ln){

	if(!exp)
		_panic("Assertion Failed",f,ln);
}

