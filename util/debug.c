/*
 * debug.c
 * Wolfrath/Kriewall, 2013
 *
 * Implementation of debug functions
 */

#include "debug.h"

void trace_stack(uint frames){

	uint * ebp = &frames - 2;
	printf("Stack trace:\n");

	uint frame;

	for(frame = 0; frame < frames; ++frame){

		uint eip = ebp[1];

		/* Check to make sure there is a caller */
		if(eip == 0)
			break;

		/* Go back one more frame */
		ebp = (uint*)ebp[0];
		uint * args = &ebp[2];
		printf("  0x%p\n", eip);
	}
}

void _debug(char* msg, char* f, uint ln){

	printf("\nDEBUG (%s at line %d)> %s\n", f, ln, msg);
}

void _panic(char* msg, char* f, uint ln){

	printf("\nPANIC (%s at line %d)> %s", f, ln, msg);
	for(;;)
		;
}

void _assert(uint exp, char* f, uint ln){

	if(!exp)
		_panic("Assertion Failed",f,ln);
}

