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

/* Generic handler called from asm */
void isr_handler(w_regs regs){

	/* Is there a handler registered? */
	if (interrupt_handlers[regs.int_no] != 0){

		w_isr handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
	else{
		printf("Recieved Interrupt 0x%p\n", regs.int_no);
	}
}

/* Generic handler called from asm */
void irq_handler(w_regs regs){

	/* Send an EOI signal to the PIC */
	
	if (regs.int_no >= 40){

		/* Send reset signal to slave */
		out_byte(0xA0, 0x20);
	}

	/* Reset master */
	out_byte(0x20, 0x20);

	if (interrupt_handlers[regs.int_no] != 0){

		w_isr handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
}
