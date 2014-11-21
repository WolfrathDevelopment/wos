/*
 * paging.c
 Joel Wolfrath, 2013
 *
 * Implementation of paging functions
 */

#include "mem.h"
#include "../core/core.h"
#include "../util/debug.h"

/* Initial page table(s) */

__attribute__((__aligned__(PAGE_SIZE)))
w_pte init_pgtbl[NUM_PTE];


/* Initial page directory */

__attribute__((__aligned__(PAGE_SIZE)))
w_pde init_pgdir[NUM_PDE];


/* Initial stack */

__attribute__((__aligned__(PAGE_SIZE)))
w_uint32 init_stack[0x800];


w_pde* current_page_directory;
w_pde* kernel_page_directory;

static void enable_paging(){

	w_uint32 cr0;

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

	w_uint32 cr0;
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 &= 0x7FFFFFFF;
	asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void set_page_directory(w_pde* dir){

	current_page_directory = dir;
	enable_paging();
}

int is_mapped(w_pde* dir, w_uint32 va){

	w_pde pde = dir[ PD_INDEX(va) ];

	if(pde & PTE_P){

		w_pte* tbl = (w_pte*) KVIRT( PTE_ADDR(pde) );
		w_pte page = tbl[ PT_INDEX(va) ];
		
		if(page & PTE_P)
			return 1;
	}

	return 0;
}


/* Map a page to the virtual address in the given page directory */

void map_page(w_pde* dir, w_uint32 va, w_pte page){

	w_pte* table;
	w_pde pde = dir[ PD_INDEX(va) ];

	if(!pde){
		PANIC("NO PAGE TABLE");
	}

	table = (w_pte*) KVIRT( PTE_ADDR(pde) );


	/* Write the physical page to the page table */

	table[ PT_INDEX(va) ] = page;
}


/* Unmap the page at the given virtual address */

void unmap_page(w_pde* dir, w_uint32 va){

	w_pte* table;
	w_pde pde = dir[ PD_INDEX(va) ];

	if(!pde){

		/* This page is not mapped */
		return;
	}

	table = (w_pte*) KVIRT( PTE_ADDR(pde) );


	/* Remove the entry in this page table */

	table[ PT_INDEX(va) ] = 0;
}

void init_paging(){

	init_lock(&mem_lock);
	register_interrupt_handler(INT_PAGEFAULT, page_fault_handler);

	kernel_page_directory = init_pgdir;

	/* Unmap the first 4MiB */
	//kernel_page_directory[0] = 0;

	/* Flush all their TLB entries */
	//int i=0;
	//for(; i < 1024; i++)
	//	invalidate_page( i * 0x1000 );

	w_uint32 cr3 = (w_uint32)kernel_page_directory;

	asm volatile("mov %0, %%cr3":: "r"(KPHYS(cr3)));
}


/* Flushes this page from the TLB */

void invalidate_page(w_uint32 va){
	asm volatile("invlpg (%0)" ::"r" (va) : "memory");
}

extern w_uint32 debug;

void page_fault_handler(struct w_regs regs){

	w_uint32 fault_addr;

	/* cr2 holds fault address */

	asm volatile("mov %%cr2, %0" : "=r" (fault_addr));

	//trace_stack(5);
	printf("Page fault at 0x%p! eip=%p\n", fault_addr,regs.eip);
	PANIC("Unhandled exception!")

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
}
