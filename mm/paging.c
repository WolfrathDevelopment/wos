/*
 * paging.c
 * Wolfrath/Kriewall, 2013
 *
 * Implementation of paging functions
 */

#include "mem.h"
#include "../core/core.h"
#include "../util/debug.h"

/* Initial page directory */
__attribute__((__aligned__(PAGE_SIZE)))
w_pde init_pgdir[NUM_PDE] = {

	/* Identity map 4MiB starting at 0 and 0xC0000000 */
	[0] = (0) | PTE_P | PTE_W | PTE_PS,
	[PD_INDEX(KMAP_START)] = (0) | PTE_P | PTE_W | PTE_PS,
};

static w_pde* current_page_directory;
static w_pde* kernel_page_directory;

static void enable_paging(){

	uint cr0;
	
	// Give cr3 the address of this page directory
	asm volatile("mov %0, %%cr3":: "r"(current_page_directory));

	// Read the value currently in cr0
	asm volatile("mov %%cr0, %0": "=r"(cr0));

	// Set the paging bit
	cr0 |= 0x80000000;

	// Write new cr0 value back to the register
	asm volatile("mov %0, %%cr0":: "r"(cr0));

	// Paging is now enabled!!
}

static void disable_paging(){

	uint cr0;

	// Read the value currently in cr0
	asm volatile("mov %%cr0, %0": "=r"(cr0));

	// Unset the paging bit
	cr0 &= 0x7FFFFFFF;

	// Write new cr0 value back to the register
	asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void set_page_directory(w_pde* dir){

	current_page_directory = dir;
}

void init_paging(){

	init_lock(&mem_lock);
	register_interrupt_handler(14, page_fault_handler);

	/* Invalidate the identity mapped pages 0 - 4MiB */
}

/* Flushes this page from the TLB */
void invalidate_page(uint addr){
	asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

void page_fault_handler(w_regs regs){

	uint fault_addr;

	/* cr2 holds fault address */
	asm volatile("mov %%cr2, %0" : "=r" (fault_addr));

	int exist = !(regs.err_code & 0x1);
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

	printf("Page fault at 0x%p!\n", fault_addr);
	_panic("Unhandled exception!");
}
