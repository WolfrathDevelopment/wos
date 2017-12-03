/*
 * syscall.c
 * Joel Wolfrath, 2013
 * 
 * Implementation of syscall functions
 */

#include <proc/syscall.h>
#include <boot/boot.h>

static void sys_handler(Registers* regs){

	/* Sanity check */
//	if(regs->eax >= NUM_SYSCALLS)
//		return;

}

void init_syscalls(){

//	register_interrupt_handler(0x80,&sys_handler);
}
