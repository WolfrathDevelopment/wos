/*
 * main.c
 * Wolfrath/Kriewall, 2013
 * 
 * Kernel entry into C code
 */

#include "core/core.h"
#include "mm/mem.h"
#include "drivers/drivers.h"

int main(w_multiboot_info *mboot_ptr){

	clear_screen();
	read_mmap(mboot_ptr);

	/* Set up segments */
	init_seg();
	init_idt();
	sti();

	kbd_install();
	init_paging();

	// Lets cause a page fault!!!
	uint *ptr = (uint*)0xA0000000;
   	uint do_page_fault = *ptr;

	_panic("End of Main");
}
