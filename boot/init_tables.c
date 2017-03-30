/*
 * init_talbes.c
 * Joel Wolfrath, 2013
 *
 * Initializes...
 * 	GDT
 *	IDT
 *	ISR/IRQ Handlers
 */

#include "boot.h"

static void set_gdt(int, uint32, uint32, uint8, uint8);
static void set_idt(uint8, uint32, uint16, uint8);


/* The new GDT */

GlobalDescTableEntry	gdt_entries[7];
GlobalDescTablePointer	g_ptr;


/* The new IDT */

InterruptDescTableEntry		idt_entries[256];
InterruptDescTablePointer	i_ptr;


/* Task state of current task */

TaskStateSegment current_tss;
extern InterruptServiceRoutine	interrupt_handlers[];


/* Segment flags */

#define SEG_P     	(0x80)		/* Is Present? 			*/
#define SEG_TSS		(0x01)		/* Is TSS? 				*/
#define SEG_SEG		(0x10)		/* Is Segment? 			*/
#define SEG_X		(0x08)		/* Executable? 			*/
#define SEG_W		(0x02)		/* Writable? 			*/
	
#define SEG_AVAIL	(0x10)		/* Is Available? 		*/
#define SEG_32		(0x40)		/* Is 32 bit? 			*/
#define SEG_GBYTE	(0x00)		/* Granularity byte?	*/
#define SEG_GPAGE	(0x80)		/* Granularity page?	*/

#define SEG_KERN	(0x00)		/* Kernel permissions? 	*/
#define SEG_USER	(0x60)		/* User permissions? 	*/


void set_tss(uint32 esp){

    current_tss.esp0 = esp;
    current_tss.cs   = 0x08; // 0x0b for user mode
    current_tss.ss = current_tss.ds = current_tss.es = 0x10; 
	current_tss.fs = current_tss.gs = 0x10; // 0x13 for user mode ^
}

/* Initialize segments */

static void seg_init(){

	g_ptr.limit = (sizeof(GlobalDescTableEntry) * 5) - 1;
	g_ptr.base  = (uint32)&gdt_entries;

	uint32 tmp_access;
	uint32 tmp_gran;

	/* Segment 0 - Null Segment */

	set_gdt(0, 0, 0, 0, 0);


	/* Segment 1 - Kernel Code Segment */

	tmp_access = SEG_P | SEG_W | SEG_SEG | SEG_KERN | SEG_X;
	tmp_gran = SEG_32 | SEG_GPAGE;
	set_gdt(1, 0, 0xFFFFFFFF,  tmp_access, tmp_gran);


	/* Segment 2 - Kernel Data Segment */

	tmp_access = SEG_P | SEG_W | SEG_SEG | SEG_KERN;
	tmp_gran = SEG_32 | SEG_GPAGE;
	set_gdt(2, 0, 0xFFFFFFFF,  tmp_access, tmp_gran);


	/* Segment 3 User Code Segment */

	tmp_access = SEG_P | SEG_W | SEG_SEG | SEG_USER | SEG_X;
	tmp_gran = SEG_32 | SEG_GPAGE | SEG_AVAIL;
	set_gdt(3, 0, 0xFFFFFFFF,  tmp_access, tmp_gran);


	/* Segment 4 - User Data Segment */

	tmp_access = SEG_P | SEG_W | SEG_SEG | SEG_USER;
	tmp_gran = SEG_32 | SEG_GPAGE | SEG_AVAIL;
	set_gdt(4, 0, 0xFFFFFFFF,  tmp_access, tmp_gran);


	/* Segment 5 - User TLS Segment */

	tmp_access = SEG_P | SEG_W | SEG_SEG | SEG_USER;
	tmp_gran = SEG_32 | SEG_GPAGE | SEG_AVAIL;
	set_gdt(5, 0, 0xFFFFFFFF,  tmp_access, tmp_gran);


	/* Segment 6 - TSS Segment */

	tmp_access = SEG_P | SEG_KERN | SEG_X | SEG_TSS;
	tmp_gran = SEG_32 | SEG_GBYTE;
	uint32 addr = (uint32)&current_tss;

	set_gdt(6, addr, sizeof(TaskStateSegment), tmp_access, tmp_gran);


	gdt_flush(&g_ptr);
}

static void idt_init(){

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

void init_seg(){

	seg_init();
	idt_init();
	zero(&interrupt_handlers, sizeof(InterruptServiceRoutine) * 256);
	//tss_flush();
}

/* Set the value of one GDT entry */

static void set_gdt(int n,uint32 base,uint32 lim,uint8 a,uint8 gran){

	gdt_entries[n].base_low = (base & 0xFFFF);
	gdt_entries[n].base_middle = (base >> 16) & 0xFF;
	gdt_entries[n].base_high = (base >> 24) & 0xFF;

	gdt_entries[n].limit_low = (lim & 0xFFFF);
	gdt_entries[n].granularity = (lim >> 16) & 0x0F;

	gdt_entries[n].granularity |= gran & 0xF0;
	gdt_entries[n].access = a;
}

void init_idt(){

}

static void set_idt(uint8 num, uint32 base, uint16 sel, uint8 flags){

    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel     = sel;
    idt_entries[num].always0 = 0;

    /* OR with 0x60 for user mode for privilege level 3 */

    idt_entries[num].flags   = flags /* | 0x60 */;
}
