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

OsIsr gbl_interrupt_handlers[OsIsrMaxId];

/* The new IDT */

InterruptDescTableEntry     idt_entries[OsIsrMaxId];
InterruptDescTablePointer   i_ptr;

/* The current process structure */

extern struct w_proc* current_proc;

static void set_idt(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt_entries[num].base_addr_low = base & 0xFFFF;
    idt_entries[num].base_addr_high = (base >> 16) & 0xFFFF;

    idt_entries[num].selector = sel;
    idt_entries[num].always0 = 0;

    /* OR with 0x60 for user mode for privilege level 3 */

    idt_entries[num].flags   = flags /* | 0x60 */;
}

/* Add interrupt handler to interrupt table */

OsRc isr_register(OsIsrId id, OsIsr routine)
{
    ASSERT(gbl_interrupt_handlers[id] == 0); // Don't trash an existing isr
    OsRc rc = RC_SUCCESS;

    if(!routine || id >= OsIsrMaxId)
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
     OsIsr handler = gbl_interrupt_handlers[regs.int_no];
    if (handler)
    {
        handler(&regs);
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
        write_io_bus_port(IO_BUS_SLAVE_PIC_CONTROL_REG, 0x20);
    }

    /* Reset master */
    write_io_bus_port(IO_BUS_MASTER_PIC_CONTROL_REG, 0x20);

    OsIsr handler = gbl_interrupt_handlers[regs.int_no];
    if(handler)
    {
        if(regs.int_no == OsIsrPic)
        {
            /* Context switch! */
            if(current_proc != NULL)
            {
                schedule();
            }
        }

        handler(&regs);
    }
}

OsRc idt_init()
{
    OsRc rc = RC_SUCCESS;

    zero(&gbl_interrupt_handlers, sizeof(OsIsr) * OsIsrMaxId);

    i_ptr.limit = sizeof(InterruptDescTableEntry) * OsIsrMaxId - 1;
    i_ptr.base  = (uint32_t)&idt_entries;

    zero(&idt_entries, sizeof(InterruptDescTableEntry) * OsIsrMaxId);

    /* Remap IRQ table */

    write_io_bus_port(IO_BUS_MASTER_PIC_CONTROL_REG, 0x11);
    write_io_bus_port(IO_BUS_SLAVE_PIC_CONTROL_REG, 0x11);
    write_io_bus_port(IO_BUS_MASTER_PIC_DATA_REG, 0x20);
    write_io_bus_port(IO_BUS_SLAVE_PIC_DATA_REG, 0x28);
    write_io_bus_port(IO_BUS_MASTER_PIC_DATA_REG, 0x04);
    write_io_bus_port(IO_BUS_SLAVE_PIC_DATA_REG, 0x02);
    write_io_bus_port(IO_BUS_MASTER_PIC_DATA_REG, 0x01);
    write_io_bus_port(IO_BUS_SLAVE_PIC_DATA_REG, 0x01);
    write_io_bus_port(IO_BUS_MASTER_PIC_DATA_REG, 0x00);
    write_io_bus_port(IO_BUS_SLAVE_PIC_DATA_REG, 0x00);

    set_idt( 0, (uint32_t)isr0 , SEG_KERNEL_CODE, 0x8E);
    set_idt( 1, (uint32_t)isr1 , SEG_KERNEL_CODE, 0x8E);
    set_idt( 2, (uint32_t)isr2 , SEG_KERNEL_CODE, 0x8E);
    set_idt( 3, (uint32_t)isr3 , SEG_KERNEL_CODE, 0x8E);
    set_idt( 4, (uint32_t)isr4 , SEG_KERNEL_CODE, 0x8E);
    set_idt( 5, (uint32_t)isr5 , SEG_KERNEL_CODE, 0x8E);
    set_idt( 6, (uint32_t)isr6 , SEG_KERNEL_CODE, 0x8E);
    set_idt( 7, (uint32_t)isr7 , SEG_KERNEL_CODE, 0x8E);
    set_idt( 8, (uint32_t)isr8 , SEG_KERNEL_CODE, 0x8E);
    set_idt( 9, (uint32_t)isr9 , SEG_KERNEL_CODE, 0x8E);
    set_idt(10, (uint32_t)isr10, SEG_KERNEL_CODE, 0x8E);
    set_idt(11, (uint32_t)isr11, SEG_KERNEL_CODE, 0x8E);
    set_idt(12, (uint32_t)isr12, SEG_KERNEL_CODE, 0x8E);
    set_idt(13, (uint32_t)isr13, SEG_KERNEL_CODE, 0x8E);
    set_idt(14, (uint32_t)isr14, SEG_KERNEL_CODE, 0x8E);
    set_idt(15, (uint32_t)isr15, SEG_KERNEL_CODE, 0x8E);
    set_idt(16, (uint32_t)isr16, SEG_KERNEL_CODE, 0x8E);
    set_idt(17, (uint32_t)isr17, SEG_KERNEL_CODE, 0x8E);
    set_idt(18, (uint32_t)isr18, SEG_KERNEL_CODE, 0x8E);
    set_idt(19, (uint32_t)isr19, SEG_KERNEL_CODE, 0x8E);
    set_idt(20, (uint32_t)isr20, SEG_KERNEL_CODE, 0x8E);
    set_idt(21, (uint32_t)isr21, SEG_KERNEL_CODE, 0x8E);
    set_idt(22, (uint32_t)isr22, SEG_KERNEL_CODE, 0x8E);
    set_idt(23, (uint32_t)isr23, SEG_KERNEL_CODE, 0x8E);
    set_idt(24, (uint32_t)isr24, SEG_KERNEL_CODE, 0x8E);
    set_idt(25, (uint32_t)isr25, SEG_KERNEL_CODE, 0x8E);
    set_idt(26, (uint32_t)isr26, SEG_KERNEL_CODE, 0x8E);
    set_idt(27, (uint32_t)isr27, SEG_KERNEL_CODE, 0x8E);
    set_idt(28, (uint32_t)isr28, SEG_KERNEL_CODE, 0x8E);
    set_idt(29, (uint32_t)isr29, SEG_KERNEL_CODE, 0x8E);
    set_idt(30, (uint32_t)isr30, SEG_KERNEL_CODE, 0x8E);
    set_idt(31, (uint32_t)isr31, SEG_KERNEL_CODE, 0x8E);
    set_idt(32, (uint32_t)irq0, SEG_KERNEL_CODE, 0x8E);
    set_idt(33, (uint32_t)irq1, SEG_KERNEL_CODE, 0x8E);
    set_idt(34, (uint32_t)irq2, SEG_KERNEL_CODE, 0x8E);
    set_idt(35, (uint32_t)irq3, SEG_KERNEL_CODE, 0x8E);
    set_idt(36, (uint32_t)irq4, SEG_KERNEL_CODE, 0x8E);
    set_idt(37, (uint32_t)irq5, SEG_KERNEL_CODE, 0x8E);
    set_idt(38, (uint32_t)irq6, SEG_KERNEL_CODE, 0x8E);
    set_idt(39, (uint32_t)irq7, SEG_KERNEL_CODE, 0x8E);
    set_idt(40, (uint32_t)irq8, SEG_KERNEL_CODE, 0x8E);
    set_idt(41, (uint32_t)irq9, SEG_KERNEL_CODE, 0x8E);
    set_idt(42, (uint32_t)irq10, SEG_KERNEL_CODE, 0x8E);
    set_idt(43, (uint32_t)irq11, SEG_KERNEL_CODE, 0x8E);
    set_idt(44, (uint32_t)irq12, SEG_KERNEL_CODE, 0x8E);
    set_idt(45, (uint32_t)irq13, SEG_KERNEL_CODE, 0x8E);
    set_idt(46, (uint32_t)irq14, SEG_KERNEL_CODE, 0x8E);
    set_idt(47, (uint32_t)irq15, SEG_KERNEL_CODE, 0x8E);

    idt_flush(&i_ptr);

    return rc;
}
