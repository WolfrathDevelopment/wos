#include "mem.h"

void init_paging(){
	register_interrupt_handler(14, page_fault_handler);
}

void page_fault_handler(Registers regs){

	//fault address is in reg CR2
	uint fault_addr;
	asm volatile("mov %%cr2, %0" : "=r" (fault_addr));

	//error details
	int exist = !(regs.err_code & 0x1);
	//write operation?
	int rw = regs.err_code & 0x2;
	int us = regs.err_code & 0x4;
	int res = regs.err_code & 0x8;
	int id = regs.err_code & 0x10;

	if(exist){
		
	}
	if(rw){
	
	}
	if(us){
		
	}
	if(res){
		
	}
}
