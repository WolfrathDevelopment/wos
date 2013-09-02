/*
 * cpu.c
 * Wolfrath/Kriewall, 2013
 *
 * Impelentation of cpu functions
 */

#include "core.h"

inline void cli(void){
	asm volatile("cli");
}

inline void sti(void){
	asm volatile("sti");
}
