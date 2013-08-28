#include "mem.h"

void page_fault_handler(Registers regs){

	//fault address is stored in register CR2
	uint fault_addr;
	asm volatile("mov %%cr2, %0" : "=r" (fault_addr));

	//error details
	int exist = !(regs.err_code & 0x1);
	int rw = regs.err_code & 0x2;
	int us = regs.err_code & 0x4;
	int res = regs.err_code & 0x8;
	int id = regs.err_code & 0x10;

	if(exist){
		/* Page already present */
	}
	if(rw){
		/* Page is read only */
	}
	if(us){
		/* Attempted write from user mode */
	}
	if(res){
		/* Page is reserved */
	}

	//put_hex(fault_addr);
}
