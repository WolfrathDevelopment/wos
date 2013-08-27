// wOS - Joel Wolfrath, 2012
// main.c - kernel entrypoint into c.

#include "core/core.h"
#include "core/boot.h"

int main(){

	init_descriptor_tables();
	clear_screen();
	put_string("Here's to the broments");
	asm volatile("sti");
	asm volatile("int $0x3");
	for(;;)
		;
}
