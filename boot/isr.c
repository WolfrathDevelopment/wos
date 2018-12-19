/*
 * isr.c
 * Joel Wolfrath, 2013
 *
 * Default implementaion of ISR functions
 */

#include <types.h>
#include <io/bus.h>
#include <boot/boot.h>
#include <boot/isr.h>
#include <lib/string.h>
#include <proc/proc.h>

/* THE interrupt table */

InterruptServiceRoutine	gbl_interrupt_handlers[MaxIsrId];

/* The new IDT */

InterruptDescTableEntry		idt_entries[256];
InterruptDescTablePointer	i_ptr;

/* The current process structure */

extern struct w_proc* current_proc;

static void set_idt(uint8 num, uint32 base, uint16 sel, uint8 flags)
{
    idt_entries[num].base_addr_low = base & 0xFFFF;
    idt_entries[num].base_addr_high = (base >> 16) & 0xFFFF;

    idt_entries[num].selector = sel;
    idt_entries[num].always0 = 0;

    /* OR with 0x60 for user mode for privilege level 3 */

    idt_entries[num].flags   = flags /* | 0x60 */;
}

/* Add interrupt handler to interrupt table */

OsRc register_isr(InterruptId id, InterruptServiceRoutine routine)
{
    ASSERT(gbl_interrupt_handlers[id] == 0); // Don't trash an existing isr
    OsRc rc = RC_SUCCESS;

    if(!routine || id >= MaxIsrId)
    {
        rc = RC_INVALID_PARM;
    }
    else
    {
        gbl_interrupt_handlers[id] = routine;
    }

    return rc;
}

/* Generic ISR handler called from asm */
void global_isr_handler(OsIsrFrame regs)
{
    if(current_proc != NULL)
    {
        memcpy(current_proc->regs, &regs, sizeof(regs));
    }

    /* Is there a handler registered? */
	if (gbl_interrupt_handlers[regs.int_no])
    {
        InterruptServiceRoutine handler = gbl_interrupt_handlers[regs.int_no];
        handler(regs);
	}
	else
    {
		// I guess we're dropping this one on the floor...
	}
}

/* Generic IRQ handler called from asm */
void irq_handler(OsIsrFrame regs)
{
    if(current_proc != NULL)
    {
        memcpy(current_proc->regs, &regs, sizeof(regs));
	}

	/* Send an EOI signal to the PIC */

	if (regs.int_no >= 40){

		/* Send reset signal to slave */
		out_byte(0xA0, 0x20);
	}


	/* Reset master */
	out_byte(0x20, 0x20);

	if(gbl_interrupt_handlers[regs.int_no] != 0)
    {
		if(regs.int_no == INT_PIC)
        {
        	/* Context switch! */
			if(current_proc != NULL)
            {
				schedule();
			}
		}

		InterruptServiceRoutine handler = gbl_interrupt_handlers[regs.int_no];
		handler(regs);
	}
}

OsRc init_idt()
{
	zero(&gbl_interrupt_handlers, sizeof(InterruptServiceRoutine) * MaxIsrId);

	i_ptr.limit = sizeof(InterruptDescTableEntry) * 256 -1;
	i_ptr.base  = (uint32)&idt_entries;

	zero(&idt_entries, sizeof(InterruptDescTableEntry) * 256);

	/* Remap IRQ table */

	out_byte(0x20, 0x11);
	out_byte(0xA0, 0x11);
	out_byte(0x21, 0x20);
	out_byte(0xA1, 0x28);
	out_byte(0x21, 0x04);
	out_byte(0xA1, 0x02);
	out_byte(0x21, 0x01);
	out_byte(0xA1, 0x01);
	out_byte(0x21, 0x0);
	out_byte(0xA1, 0x0);

	set_idt( 0, (uint32)isr0 , SEG_KERNEL_CODE, 0x8E);
	set_idt( 1, (uint32)isr1 , SEG_KERNEL_CODE, 0x8E);
	set_idt( 2, (uint32)isr2 , SEG_KERNEL_CODE, 0x8E);
	set_idt( 3, (uint32)isr3 , SEG_KERNEL_CODE, 0x8E);
	set_idt( 4, (uint32)isr4 , SEG_KERNEL_CODE, 0x8E);
	set_idt( 5, (uint32)isr5 , SEG_KERNEL_CODE, 0x8E);
	set_idt( 6, (uint32)isr6 , SEG_KERNEL_CODE, 0x8E);
	set_idt( 7, (uint32)isr7 , SEG_KERNEL_CODE, 0x8E);
	set_idt( 8, (uint32)isr8 , SEG_KERNEL_CODE, 0x8E);
	set_idt( 9, (uint32)isr9 , SEG_KERNEL_CODE, 0x8E);
	set_idt(10, (uint32)isr10, SEG_KERNEL_CODE, 0x8E);
	set_idt(11, (uint32)isr11, SEG_KERNEL_CODE, 0x8E);
	set_idt(12, (uint32)isr12, SEG_KERNEL_CODE, 0x8E);
	set_idt(13, (uint32)isr13, SEG_KERNEL_CODE, 0x8E);
	set_idt(14, (uint32)isr14, SEG_KERNEL_CODE, 0x8E);
	set_idt(15, (uint32)isr15, SEG_KERNEL_CODE, 0x8E);
	set_idt(16, (uint32)isr16, SEG_KERNEL_CODE, 0x8E);
	set_idt(17, (uint32)isr17, SEG_KERNEL_CODE, 0x8E);
	set_idt(18, (uint32)isr18, SEG_KERNEL_CODE, 0x8E);
	set_idt(19, (uint32)isr19, SEG_KERNEL_CODE, 0x8E);
	set_idt(20, (uint32)isr20, SEG_KERNEL_CODE, 0x8E);
	set_idt(21, (uint32)isr21, SEG_KERNEL_CODE, 0x8E);
	set_idt(22, (uint32)isr22, SEG_KERNEL_CODE, 0x8E);
	set_idt(23, (uint32)isr23, SEG_KERNEL_CODE, 0x8E);
	set_idt(24, (uint32)isr24, SEG_KERNEL_CODE, 0x8E);
	set_idt(25, (uint32)isr25, SEG_KERNEL_CODE, 0x8E);
	set_idt(26, (uint32)isr26, SEG_KERNEL_CODE, 0x8E);
	set_idt(27, (uint32)isr27, SEG_KERNEL_CODE, 0x8E);
	set_idt(28, (uint32)isr28, SEG_KERNEL_CODE, 0x8E);
	set_idt(29, (uint32)isr29, SEG_KERNEL_CODE, 0x8E);
    set_idt(30, (uint32)isr30, SEG_KERNEL_CODE, 0x8E);
	set_idt(31, (uint32)isr31, SEG_KERNEL_CODE, 0x8E);
	set_idt(32, (uint32)irq0, SEG_KERNEL_CODE, 0x8E);
	set_idt(33, (uint32)irq1, SEG_KERNEL_CODE, 0x8E);
	set_idt(34, (uint32)irq2, SEG_KERNEL_CODE, 0x8E);
	set_idt(35, (uint32)irq3, SEG_KERNEL_CODE, 0x8E);
	set_idt(36, (uint32)irq4, SEG_KERNEL_CODE, 0x8E);
	set_idt(37, (uint32)irq5, SEG_KERNEL_CODE, 0x8E);
	set_idt(38, (uint32)irq6, SEG_KERNEL_CODE, 0x8E);
	set_idt(39, (uint32)irq7, SEG_KERNEL_CODE, 0x8E);
	set_idt(40, (uint32)irq8, SEG_KERNEL_CODE, 0x8E);
	set_idt(41, (uint32)irq9, SEG_KERNEL_CODE, 0x8E);
	set_idt(42, (uint32)irq10, SEG_KERNEL_CODE, 0x8E);
	set_idt(43, (uint32)irq11, SEG_KERNEL_CODE, 0x8E);
	set_idt(44, (uint32)irq12, SEG_KERNEL_CODE, 0x8E);
	set_idt(45, (uint32)irq13, SEG_KERNEL_CODE, 0x8E);
	set_idt(46, (uint32)irq14, SEG_KERNEL_CODE, 0x8E);
	set_idt(47, (uint32)irq15, SEG_KERNEL_CODE, 0x8E);

	idt_flush(&i_ptr);
}
