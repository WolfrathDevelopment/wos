/*
 * main.c
 * Wolfrath/Kriewall, 2013
 * 
 * Kernel entry into C code
 */

#include "core/core.h"
#include "mm/mem.h"

int main(Multiboot_info *mboot_ptr){

	clear_screen();
	read_mmap(mboot_ptr);

	/* Set up segments */
	init_seg();

	/* Set up idt */
	init_idt();
	sti();
	
	for(;;)
		;
}
