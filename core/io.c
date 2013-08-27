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

/* Write a byte to the specified port */
void out_byte(ushort port, uchar val){

	asm volatile("out %1, %0" : : "dN" (port), "a" (val));
}

/* Read a byte from the specified port */
uchar in_byte(ushort port){

    uchar val;
    asm volatile("in %1, %0" : "=a" (val) : "dN" (port));
    return val;
}

/* Read two bytes from the specified port */
ushort in_short(ushort port){

    ushort val;
    asm volatile("in %1, %0" : "=a" (val) : "dN" (port));
    return val;
}
