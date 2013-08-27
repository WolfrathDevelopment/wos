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
extern w_isr interrupt_handlers[];

// Initialisation routine - zeroes all the interrupt service routines,
// initialises the GDT and IDT.
void init_descriptor_tables(){

	init_gdt();
	init_idt();
	memset(&interrupt_handlers, 0, sizeof(w_isr)*256);
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
static void set_gdt(int num, uint base, uint limit, uchar access, uchar gran){

	gdt_entries[num].base_low = (base & 0xFFFF);
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_high = (base >> 24) & 0xFF;

	gdt_entries[num].limit_low = (limit & 0xFFFF);
	gdt_entries[num].granularity = (limit >> 16) & 0x0F;
    
	gdt_entries[num].granularity |= gran & 0xF0;
	gdt_entries[num].access = access;
}

static void init_idt(){

	i_ptr.limit = sizeof(IDT_e) * 256 -1;
	i_ptr.base  = (uint)&idt_entries;

	memset(&idt_entries, 0, sizeof(IDT_e)*256);

	// Remap the irq table.
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

	set_idt( 0, (uint)isr0 , 0x08, 0x8E);
	set_idt( 1, (uint)isr1 , 0x08, 0x8E);
	set_idt( 2, (uint)isr2 , 0x08, 0x8E);
	set_idt( 3, (uint)isr3 , 0x08, 0x8E);
	set_idt( 4, (uint)isr4 , 0x08, 0x8E);
	set_idt( 5, (uint)isr5 , 0x08, 0x8E);
	set_idt( 6, (uint)isr6 , 0x08, 0x8E);	
	set_idt( 7, (uint)isr7 , 0x08, 0x8E);
	set_idt( 8, (uint)isr8 , 0x08, 0x8E);
	set_idt( 9, (uint)isr9 , 0x08, 0x8E);	
	set_idt(10, (uint)isr10, 0x08, 0x8E);
	set_idt(11, (uint)isr11, 0x08, 0x8E);
	set_idt(12, (uint)isr12, 0x08, 0x8E);
	set_idt(13, (uint)isr13, 0x08, 0x8E);
	set_idt(14, (uint)isr14, 0x08, 0x8E);
	set_idt(15, (uint)isr15, 0x08, 0x8E);
	set_idt(16, (uint)isr16, 0x08, 0x8E);
	set_idt(17, (uint)isr17, 0x08, 0x8E);	
	set_idt(18, (uint)isr18, 0x08, 0x8E);
	set_idt(19, (uint)isr19, 0x08, 0x8E);
	set_idt(20, (uint)isr20, 0x08, 0x8E);
	set_idt(21, (uint)isr21, 0x08, 0x8E);
	set_idt(22, (uint)isr22, 0x08, 0x8E);
	set_idt(23, (uint)isr23, 0x08, 0x8E);
	set_idt(24, (uint)isr24, 0x08, 0x8E);
	set_idt(25, (uint)isr25, 0x08, 0x8E);
	set_idt(26, (uint)isr26, 0x08, 0x8E);
	set_idt(27, (uint)isr27, 0x08, 0x8E);	
	set_idt(28, (uint)isr28, 0x08, 0x8E);
	set_idt(29, (uint)isr29, 0x08, 0x8E);
	set_idt(30, (uint)isr30, 0x08, 0x8E);
	set_idt(31, (uint)isr31, 0x08, 0x8E);
	set_idt(32, (uint)irq0, 0x08, 0x8E);
	set_idt(33, (uint)irq1, 0x08, 0x8E);
	set_idt(34, (uint)irq2, 0x08, 0x8E);
	set_idt(35, (uint)irq3, 0x08, 0x8E);
	set_idt(36, (uint)irq4, 0x08, 0x8E);
	set_idt(37, (uint)irq5, 0x08, 0x8E);
	set_idt(38, (uint)irq6, 0x08, 0x8E);
	set_idt(39, (uint)irq7, 0x08, 0x8E);
	set_idt(40, (uint)irq8, 0x08, 0x8E);
	set_idt(41, (uint)irq9, 0x08, 0x8E);
	set_idt(42, (uint)irq10, 0x08, 0x8E);
	set_idt(43, (uint)irq11, 0x08, 0x8E);
	set_idt(44, (uint)irq12, 0x08, 0x8E);	
	set_idt(45, (uint)irq13, 0x08, 0x8E);
	set_idt(46, (uint)irq14, 0x08, 0x8E);
	set_idt(47, (uint)irq15, 0x08, 0x8E);

	idt_flush((uint)&i_ptr);
}

static void set_idt(uchar num, uint base, ushort sel, uchar flags){

    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel     = sel;
    idt_entries[num].always0 = 0;

    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt_entries[num].flags   = flags /* | 0x60 */;
}
