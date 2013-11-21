/*
 * main.c
 * Wolfrath/Kriewall, 2013
 *
 * Kernel entry into C code
 */

#include "core/core.h"
#include "mm/mem.h"
#include "drivers/drivers.h"
#include "util/debug.h"
#include "proc/proc.h"

extern w_pde* kernel_page_directory;
extern w_pte init_pgtbl[];
extern struct w_tss current_tss;

uint core_stack;
uint debug = 0;

static void remap_stack(uint* ebp){

	/* Map the stack to proper virtual address */
}

int main(uint* ebp, w_multiboot_info* mboot_ptr){

	clear_screen();
	read_mmap(mboot_ptr);

	/* Set up segments */
	init_seg();
	init_idt();
	sti();

	kbd_install();
	init_alloc(mboot_ptr);

    
	//remap_stack(ebp);

	/* Set stack guard page */
	unmap_page(kernel_page_directory, ((uint)&ebp) - 0x1000);

    
	/* This will unmap the first 4 MiB */
	init_paging();

    
	init_kheap();

    	/* Heap test allocations */
	uint* n1, *n2, *n3;
	n1 = kalloc(sizeof(uint));
	n2 = kalloc(sizeof(uint));
	n3 = kalloc(sizeof(uint));

	*n1 = *n2 = *n3 = 256;

	printf("New int value: 0x%p at addr: 0x%p\n",*n1, n1);

	kfree((uint)n1);
	kfree((uint)n2);

	//PANIC("END OF MAIN");
	kfree((uint)n3);

	init_pic(50);

	// Lets cause a page fault!!!
	//uint *ptr = (uint*)0x00a00000;
   	//uint do_page_fault = *ptr;

	begin_multitasking();

	PANIC("End of Main");
}
