/*
 * paging.c
 * Wolfrath/Kriewall, 2013
 *
 * Implementation of paging functions
 */

#include "mem.h"

w_pde* current_page_directory;

void set_page_directory(w_pde* dir){

	current_page_directory = dir;
	uint cr0;
	
	// Give cr3 the address of this page directory
	asm volatile("mov %0, %%cr3":: "r"(dir));

	// Read the value currently in cr0
	asm volatile("mov %%cr0, %0": "=r"(cr0));

	// Set the paging bit
	cr0 |= 0x80000000;

	// Write new cr0 value back to the register
	asm volatile("mov %0, %%cr0":: "r"(cr0));

	// Paging is now enabled!!
}

 /* Allocate a page at the given physical address */
w_pte* alloc_page(uint * addr, uint flags){

	uint result = 0;	

	// Make sure we are page aligned
	if(!PAGE_ALIGNED(addr))
		PAGE_ALIGN(addr);

	/*
	 * Zero out the page.
 	 * Since page is 4kb, we only need to iterate
	 * up to PAGE_SIZE / sizeof(int)
	 */
	int i = 0;
	uint *it = addr;
	while( i++ < PAGE_SIZE / 4){
		*it++ = NULL;
	}

	// First 20 bits are address, last 12 are flags
	// Cast to uint to avoid pointer arithmetic
	result = (uint)addr;

	// For sanity's sake, lets force the flags into 12 bits
	flags &= 0xFFF;
	return (w_pte*)(result | flags);
}

w_pde* allocate_page_directory(int is_kernel){

	w_pde* dir;

	if(is_kernel){
		
		/* Allocate the page directory */
		dir = (w_pde*) kmalloc(NUM_PDE * sizeof(w_pde), FLAG_ALIGN);

		int i;
		for(i = 0; i<NUM_PDE; i++){
			
			/* Allocate a page table */			
			w_pte * table = (w_pte*) kmalloc(NUM_PTE * sizeof(w_pte), FLAG_ALIGN);

			/* Point the directory entry at this new table */
			dir[i] |= ((uint) table | PTE_W); 

			/* Page table entries will get set once pages are mapped */
		}
	}
}

void init_paging(){

	/* Allocate the kernel page directory */
	current_page_directory = allocate_page_directory(1);

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
