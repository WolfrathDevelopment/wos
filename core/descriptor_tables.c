// wOS - Joel Wolfrath, 2012
// descriptor_tables.c - Initialises the GDT and IDT, and defines the 
// default ISR and IRQ handler.

#include "core.h"
#include "boot.h"

//defined in assembly
extern void gdt_flush(uint);
extern void idt_flush(uint);

static void init_gdt();
static void init_idt();
static void set_gdt(int,uint,uint,uchar,uchar);
static void set_idt(uchar,uint,ushort,uchar);

GDT_e gdt_entries[5];
GDT_p   g_ptr;
IDT_e idt_entries[256];
IDT_p   i_ptr;

// Extern the ISR handler array so we can nullify them on startup.
extern isr interrupt_handlers[];

// Initialisation routine - zeroes all the interrupt service routines,
// initialises the GDT and IDT.
void init_descriptor_tables(){

    init_gdt();
    init_idt();
    memset(&interrupt_handlers, 0, sizeof(isr)*256);
}

static void init_gdt(){

    g_ptr.limit = (sizeof(GDT_e) * 5) - 1;
    g_ptr.base  = (uint)&gdt_entries;

    set_gdt(0, 0, 0, 0, 0);                // Null segment
    set_gdt(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    set_gdt(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    set_gdt(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    set_gdt(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

    gdt_flush((uint)&g_ptr);
}

// Set the value of one GDT entry.
static void gdt_set_gate(s32int num, u32int base, u32int limit, u8int access, u8int gran)
{
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;
    
    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access      = access;
}

static void init_idt()
{
    i_ptr.limit = sizeof(idt_entry) * 256 -1;
    i_ptr.base  = (u32int)&idt_entries;

    memset(&idt_entries, 0, sizeof(idt_entry)*256);

    // Remap the irq table.
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    idt_set_gate( 0, (u32int)isr0 , 0x08, 0x8E);
    idt_set_gate( 1, (u32int)isr1 , 0x08, 0x8E);
    idt_set_gate( 2, (u32int)isr2 , 0x08, 0x8E);
    idt_set_gate( 3, (u32int)isr3 , 0x08, 0x8E);
    idt_set_gate( 4, (u32int)isr4 , 0x08, 0x8E);
    idt_set_gate( 5, (u32int)isr5 , 0x08, 0x8E);
    idt_set_gate( 6, (u32int)isr6 , 0x08, 0x8E);
    idt_set_gate( 7, (u32int)isr7 , 0x08, 0x8E);
    idt_set_gate( 8, (u32int)isr8 , 0x08, 0x8E);
    idt_set_gate( 9, (u32int)isr9 , 0x08, 0x8E);
    idt_set_gate(10, (u32int)isr10, 0x08, 0x8E);
    idt_set_gate(11, (u32int)isr11, 0x08, 0x8E);
    idt_set_gate(12, (u32int)isr12, 0x08, 0x8E);
    idt_set_gate(13, (u32int)isr13, 0x08, 0x8E);
    idt_set_gate(14, (u32int)isr14, 0x08, 0x8E);
    idt_set_gate(15, (u32int)isr15, 0x08, 0x8E);
    idt_set_gate(16, (u32int)isr16, 0x08, 0x8E);
    idt_set_gate(17, (u32int)isr17, 0x08, 0x8E);
    idt_set_gate(18, (u32int)isr18, 0x08, 0x8E);
    idt_set_gate(19, (u32int)isr19, 0x08, 0x8E);
    idt_set_gate(20, (u32int)isr20, 0x08, 0x8E);
    idt_set_gate(21, (u32int)isr21, 0x08, 0x8E);
    idt_set_gate(22, (u32int)isr22, 0x08, 0x8E);
    idt_set_gate(23, (u32int)isr23, 0x08, 0x8E);
    idt_set_gate(24, (u32int)isr24, 0x08, 0x8E);
    idt_set_gate(25, (u32int)isr25, 0x08, 0x8E);
    idt_set_gate(26, (u32int)isr26, 0x08, 0x8E);
    idt_set_gate(27, (u32int)isr27, 0x08, 0x8E);
    idt_set_gate(28, (u32int)isr28, 0x08, 0x8E);
    idt_set_gate(29, (u32int)isr29, 0x08, 0x8E);
    idt_set_gate(30, (u32int)isr30, 0x08, 0x8E);
    idt_set_gate(31, (u32int)isr31, 0x08, 0x8E);
    idt_set_gate(32, (u32int)irq0, 0x08, 0x8E);
    idt_set_gate(33, (u32int)irq1, 0x08, 0x8E);
    idt_set_gate(34, (u32int)irq2, 0x08, 0x8E);
    idt_set_gate(35, (u32int)irq3, 0x08, 0x8E);
    idt_set_gate(36, (u32int)irq4, 0x08, 0x8E);
    idt_set_gate(37, (u32int)irq5, 0x08, 0x8E);
    idt_set_gate(38, (u32int)irq6, 0x08, 0x8E);
    idt_set_gate(39, (u32int)irq7, 0x08, 0x8E);
    idt_set_gate(40, (u32int)irq8, 0x08, 0x8E);
    idt_set_gate(41, (u32int)irq9, 0x08, 0x8E);
    idt_set_gate(42, (u32int)irq10, 0x08, 0x8E);
    idt_set_gate(43, (u32int)irq11, 0x08, 0x8E);
    idt_set_gate(44, (u32int)irq12, 0x08, 0x8E);
    idt_set_gate(45, (u32int)irq13, 0x08, 0x8E);
    idt_set_gate(46, (u32int)irq14, 0x08, 0x8E);
    idt_set_gate(47, (u32int)irq15, 0x08, 0x8E);

    idt_flush((u32int)&i_ptr);
}

static void idt_set_gate(u8int num, u32int base, u16int sel, u8int flags)
{
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel     = sel;
    idt_entries[num].always0 = 0;
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt_entries[num].flags   = flags /* | 0x60 */;
}
