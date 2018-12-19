/*
 * main.c
 * Joel Wolfrath, 2013
 *
 * Definitions for main C code
 */

#include <boot/isr.h>
#include <lib/core.h>
#include <mm/mem.h>
#include <mm/ppa.h>
#include <drivers/drivers.h>
#include <tools/debug.h>
#include <proc/proc.h>

extern uint8_t          init_stack;
extern PageDirectory	init_pgdir;
extern struct w_tss		current_tss;
extern struct w_proc*	current_proc;

int main(GrubMultibootInfo* mboot_ptr){

	/* Set up segments */
	init_seg();
	init_idt();
	sti();

	clear_screen();
	void* addr = read_mmap(mboot_ptr);

	kbd_install();

    ppa_init(mboot_ptr);

	//printf("Current instruction: %p\n",get_eip());
	//uint32 sptr = get_esp();
	//printf("Current Stack Pointer: %p\n",sptr);

	//remap_stack(ebp);
	/* Set stack guard page */
    page_unmap(&init_pgdir, ((uint32_t)&init_stack) - PAGE_SIZE);

    ASSERT(NULL);

	/* This will unmap the first 4 MiB */
	//init_paging();
	//init_kheap();

    /* Heap test allocations */
    //uint32* n1, *n2, *n3;
    //n1 = kalloc(sizeof(uint32));
    //n2 = kalloc(sizeof(uint32));
    //n3 = kalloc(sizeof(uint32));

    //*n1 = *n2 = *n3 = 256;

    //printf("New int value: 0x%p at addr: 0x%p\n",*n1, n1);

    //kfree((uint32)n1);
    //kfree((uint32)n2);
    //kfree((uint32)n3);

	// Lets cause a page fault!!!
	//uint32 *ptr = (uint32*)0x00a00000;
   	//uint32 do_page_fault = *ptr;

//	begin_multitasking();
	//init_pic(50);

//	int32 p = fork();
//	if(p)
//		printf("Parent returned, child pid = %p\n", p);
//	else
//		printf("Child returned\n");

    //PANIC("End of Main");
}
