/*
 * isr.c
 * Wolfrath/Kriewall, 2013
 *
 * Default implementaion of ISR functions
 */

#include "boot.h"

w_isr interrupt_handlers[256];

void register_interrupt_handler(uchar n, w_isr handler){

	interrupt_handlers[n] = handler;
}

// Called from ASM interrupt handler stub.
void isr_handler(Registers regs){

	put_string("recieved interrupt: ");
	put_decimal(regs.int_no);
	put_char('\n');

	if (interrupt_handlers[regs.int_no] != 0){

		w_isr handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
}

// Called from our ASM interrupt handler stub.
void irq_handler(Registers regs){

	// Send an EOI (end of interrupt) signal to the PICs.
	// If this interrupt involved the slave...
	if (regs.int_no >= 40){
		// Send reset signal to slave.
		out_byte(0xA0, 0x20);
	}

	// Send reset signal to master
	out_byte(0x20, 0x20);

	if (interrupt_handlers[regs.int_no] != 0){

		w_isr handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
}
