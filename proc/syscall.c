/*
 * syscall.c
 * Wolfrath/Kriewall, 2013
 * 
 * Implementation of syscall functions
 */

#include "syscall.h"

static void sys_handler(struct w_regs* regs){

	/* Sanity check */
	if(regs->eax >= NUM_SYSCALLS)
		return;

}

void init_syscalls(){

	register_interrupt_handler(0x80,&sys_handler);
}
