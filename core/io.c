/*
 * io.c
 * Joel Wolfrath, 2013
 *
 * Impelentation of basic io functions on x86
 */

#include "core.h"

/*
 * Here is a use of inline assembly.  Syntax is as follows:
 *
 * asm ( assembler template 
 *         : output operands                 
 *         : input operands                   
 *         : list of clobbered registers     
 *        );
 */

void zero(void* addr, size_t size){
	
	uint8* it = addr;

	while(size--)
		*it++ = 0;
}

/* Write a byte to the specified port */
void out_byte(uint16 port, uint8 val){

	asm volatile("out %1, %0" : : "dN" (port), "a" (val));
}

/* Read a byte from the specified port */
uint8 in_byte(uint16 port){

    uint8 val;
    asm volatile("in %1, %0" : "=a" (val) : "dN" (port));
    return val;
}

/* Read two bytes from the specified port */
uint16 in_short(uint16 port){

    uint16 val;
    asm volatile("in %1, %0" : "=a" (val) : "dN" (port));
    return val;
}

inline uint32 read_eflags(void){
	uint32 eflags;
	asm volatile("pushfl; popl %0" : "=r" (eflags));
	return eflags;
}

inline void cli(void){
	asm volatile("cli");
}

inline void sti(void){
	asm volatile("sti");
}
