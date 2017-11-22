/*
 * paging.c
 Joel Wolfrath, 2013
 *
 * Implementation of paging functions
 */

#include "mem.h"
#include "paging.h"
#include "../core/core.h"
#include "debug.h"

/* Initial page table(s) */

__attribute__((__aligned__(PAGE_SIZE)))
PageTableEntry init_pgtbl[PTE_MAX_ENTRIES];


/* Initial page directory */

__attribute__((__aligned__(PAGE_SIZE)))
PageDirectoryEntry init_pgdir[PDE_MAX_ENTRIES];


/* Initial stack */

__attribute__((__aligned__(PAGE_SIZE)))
uint32 init_stack[0x800];


PageDirectoryEntry* current_page_directory;
PageDirectoryEntry* kernel_page_directory;

static void enable_paging(){

	uint32 cr0;

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

	uint32 cr0;
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 &= 0x7FFFFFFF;
	asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void set_page_directory(PageDirectoryEntry* dir){

	current_page_directory = dir;
	enable_paging();
}

int is_mapped(PageDirectoryEntry* dir, uint32 va){

	int pageDirIndex = getPDEIndex(va);
	int pageTblIndex = getPTEIndex(va);

	PageDirectoryEntry pde = dir[pageDirIndex];

	if(isPtePresent(pde)){

		PageTableEntry* tbl = (PageTableEntry*) KVIRT( PTE_ADDR(pde) );
		PageTableEntry pte = tbl[pageTblIndex];

		return isPagePresent(pte);
	}

	return FALSE;
}


/* Map a page to the virtual address in the given page directory */

void map_page(PageDirectoryEntry* dir, uint32 va, PageTableEntry page){

	PageTableEntry* table;
	PageDirectoryEntry pde = dir[ getPDEIndex(va) ];

	if(!pde){
		PANIC("NO PAGE TABLE");     // punting for now... need to allocate table
	}

	table = (PageTableEntry*) KVIRT( PTE_ADDR(pde) );

	/* Write the physical page to the page table */

	table[ getPTEIndex(va) ] = page;
}


/* Unmap the page at the given virtual address */

void unmap_page(PageDirectoryEntry* dir, uint32 va){

	PageTableEntry* table;
	PageDirectoryEntry pde = dir[ getPDEIndex(va) ];

	if(!pde){

		/* This page is not mapped */
		return;
	}

	table = (PageTableEntry*) KVIRT( PTE_ADDR(pde) );

	/* Remove the entry in this page table */

	table[ getPTEIndex(va) ] = 0;
}

void init_paging(){

	register_interrupt_handler(INT_PAGEFAULT, page_fault_handler);

	kernel_page_directory = init_pgdir;

	/* Unmap the first 4MiB */
	//kernel_page_directory[0] = 0;

	/* Flush all their TLB entries */
	//int i=0;
	//for(; i < 1024; i++)
	//	invalidate_page( i * 0x1000 );

	uint32 cr3 = (uint32)kernel_page_directory;

	asm volatile("mov %0, %%cr3":: "r"(KPHYS(cr3)));
}


/* Flushes this page from the TLB */

void invalidate_page(uint32 va){
	asm volatile("invlpg (%0)" ::"r" (va) : "memory");
}

extern uint32 debug;

void page_fault_handler(Registers regs){

	uint32 fault_addr;

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
