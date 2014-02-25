// wOS - Joel Wolfrath, 2012
// main.c - kernel entrypoint into c.

#include "core/core.h"
#include "core/boot.h"

int main(struct multiboot *mboot_ptr, uint initial_stack, uint count){

	init_descriptor_tables();
	clear_screen();
	put_string("Here's to the broments");
	put_char(' ');
	put_hex(count);
	//asm volatile("sti");
	

	

	for(;;)
		;
}
