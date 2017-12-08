/*
 * main.c
 * Joel Wolfrath, 2013
 *
 * Definitions for main C code
 */

#include <lib/core.h>
#include <mm/mem.h>
#include <drivers/drivers.h>
#include <lib/debug.h>
#include <proc/proc.h>
#include <boot/acpi.h>

extern PageDirectoryEntry*	kernel_page_directory;
extern PageTableEntry		init_pgtbl[];
extern struct w_tss		current_tss;
extern struct w_proc*		current_proc;

uint32* core_stack;

int main(uint32* ebp, GrubMultibootInfo* mboot_ptr){

	/* Set up segments */
	init_seg();
	init_idt();
	sti();

	clear_screen();
	void* addr = read_mmap(mboot_ptr);
	//struct w_rsdp* rsdp = (struct w_rsdp*)KVIRT(addr);
	//printf("addr = %p\n", rsdp);
	//printf("signature: %s\n",rsdp->signature);
	//printf("checksum: %p\n",rsdp->checksum);
	//printf("OEMID: %s\n",rsdp->oemid);
	//printf("revision: %p\n",rsdp->rev);
	//printf("rsdt_addr: %p\n",rsdp->rsdt_addr);

	//init_acpi();


	kbd_install();

	//printf("Current instruction: %p\n",get_eip());
	//uint32 sptr = get_esp();
	//printf("Current Stack Pointer: %p\n",sptr);

	//remap_stack(ebp);
	/* Set stack guard page */
	unmap_page(kernel_page_directory, ((uint32)&ebp) - 0x1000);
	core_stack = (uint32*)&ebp;

	/* This will unmap the first 4 MiB */
	init_paging();
	init_kheap();

    	/* Heap test allocations */
	uint32* n1, *n2, *n3;
	n1 = kalloc(sizeof(uint32));
	n2 = kalloc(sizeof(uint32));
	n3 = kalloc(sizeof(uint32));

	*n1 = *n2 = *n3 = 256;

	printf("New int value: 0x%p at addr: 0x%p\n",*n1, n1);

	kfree((uint32)n1);
	kfree((uint32)n2);
	kfree((uint32)n3);

	// Lets cause a page fault!!!
	uint32 *ptr = (uint32*)0x00a00000;
   	uint32 do_page_fault = *ptr;

//	begin_multitasking();
	//init_pic(50);

//	int32 p = fork();
//	if(p)
//		printf("Parent returned, child pid = %p\n", p);
//	else
//		printf("Child returned\n");

	PANIC("End of Main");
}
