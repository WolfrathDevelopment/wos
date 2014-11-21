/*
 * main.c
 * Joel Wolfrath, 2013
 *
 * Definitions for main C code
 */

#include "core/core.h"
#include "mm/mem.h"
#include "drivers/drivers.h"
#include "util/debug.h"
#include "proc/proc.h"
#include "boot/acpi.h"

extern w_pde* kernel_page_directory;
extern w_pte init_pgtbl[];
extern struct w_tss current_tss;
extern struct w_proc* current_proc;

w_uint32* core_stack;

int main(w_uint32* ebp, struct w_multiboot_info* mboot_ptr){

	clear_screen();
	w_ptr addr = read_mmap(mboot_ptr);
	//struct w_rsdp* rsdp = (struct w_rsdp*)KVIRT(addr);
	//printf("addr = %p\n", rsdp);
	//printf("signature: %s\n",rsdp->signature);
	//printf("checksum: %p\n",rsdp->checksum);
	//printf("OEMID: %s\n",rsdp->oemid);
	//printf("revision: %p\n",rsdp->rev);
	//printf("rsdt_addr: %p\n",rsdp->rsdt_addr);

	//init_acpi();

	/* Set up segments */
	init_seg();
	init_idt();
	sti();

	kbd_install();

	//printf("Current instruction: %p\n",get_eip());
	//w_uint32 sptr = get_esp();
	//printf("Current Stack Pointer: %p\n",sptr);

	//remap_stack(ebp);
	/* Set stack guard page */
	unmap_page(kernel_page_directory, ((w_uint32)&ebp) - 0x1000);
	core_stack = (w_uint32*)&ebp;

	/* This will unmap the first 4 MiB */
	init_paging();

	init_kheap();

    	/* Heap test allocations */
	w_uint32* n1, *n2, *n3;
	n1 = kalloc(sizeof(w_uint32));
	n2 = kalloc(sizeof(w_uint32));
	n3 = kalloc(sizeof(w_uint32));

	*n1 = *n2 = *n3 = 256;

	//printf("New int value: 0x%p at addr: 0x%p\n",*n1, n1);

	kfree((w_uint32)n1);
	kfree((w_uint32)n2);
	kfree((w_uint32)n3);

	// Lets cause a page fault!!!
	//uint *ptr = (uint*)0x00a00000;
   	//uint do_page_fault = *ptr;

//	begin_multitasking();
	//init_pic(50);

//	w_int32 p = fork();
//	if(p)
//		printf("Parent returned, child pid = %p\n", p);
//	else
//		printf("Child returned\n");

	PANIC("End of Main");
}
