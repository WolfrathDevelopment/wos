/*
 * isr.c
 * Joel Wolfrath, 2013
 *
 * Default implementaion of ISR functions
 */

#include "boot.h"
#include "../proc/proc.h"


/* THE interrupt table */

w_isr interrupt_handlers[256];


/* The current process structure */

extern struct w_proc* current_proc;


/* Add interrupt handler to interrupt table */

void register_interrupt_handler(w_uint8 n, w_isr handler){

	interrupt_handlers[n] = handler;
}


/* Generic ISR handler called from asm */

void isr_handler(struct w_regs regs){

    if(current_proc != NULL){
        REGS_COPY(current_proc->regs,&regs);
    }


	/* Is there a handler registered? */

	if (interrupt_handlers[regs.int_no] != 0){

		w_isr handler = interrupt_handlers[regs.int_no];
		//printf("\nRecieved Interrupt 0x%p\n", regs.int_no);
		//printf("Instruction Pointer: 0x%p\n", regs.eip);
		//PANIC("UNHANDLED INTERRUPT");

		handler(regs);
	}
	else{
		//printf("\nRecieved Interrupt 0x%p\n", regs.int_no);
		//printf("Instruction Pointer: 0x%p\n", regs.eip);
		//PANIC("UNHANDLED INTERRUPT");
	}
}


/* Generic IRQ handler called from asm */

void irq_handler(struct w_regs regs){


	if(current_proc != NULL){
		REGS_COPY(current_proc->regs,&regs);
	}

	/* Send an EOI signal to the PIC */

	if (regs.int_no >= 40){

		/* Send reset signal to slave */
		out_byte(0xA0, 0x20);
	}


	/* Reset master */

	out_byte(0x20, 0x20);

	if (interrupt_handlers[regs.int_no] != 0){

		if(regs.int_no == INT_PIC){

        	/* Context switch! */
			if(current_proc != NULL){
				schedule();
			}
		}

		w_isr handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
}
