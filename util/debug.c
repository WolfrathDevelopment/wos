#include "debug.h"

void _panic(char* msg){
	
	//put string
	for(;;)
}

void _debug(char * str){
	
	//put string
}

void _assert(int exp){
	if(!exp)
		_panic("Assertion Failed");
}

