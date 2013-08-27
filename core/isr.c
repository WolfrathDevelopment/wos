//
// isr.c -- High level interrupt service routines and interrupt request handlers.
//          Part of this code is modified from Bran's kernel development tutorials.
//          Rewritten for JamesM's kernel development tutorials.
//

#include "core.h"
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
