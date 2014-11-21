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

static void set_gdt(int, w_uint32, w_uint32, w_uint8, w_uint8);
static void set_idt(w_uint8, w_uint32, w_uint16, w_uint8);


/* The new GDT */

struct w_gdte gdt_entries[7];
struct w_gdtp g_ptr;


/* The new IDT */

struct w_idte idt_entries[256];
struct w_idtp i_ptr;


/* Task state of current task */

struct w_tss current_tss;
extern w_isr interrupt_handlers[];


/* Segment flags */

#define SEG_P     	0x80		/* Is Present? 			*/
#define SEG_TSS		0x01		/* Is TSS? 				*/
#define SEG_SEG		0x10		/* Is Segment? 			*/
#define SEG_X		0x08		/* Executable? 			*/
#define SEG_W		0x02		/* Writable? 			*/
	
#define SEG_AVAIL	0x10		/* Is Available? 		*/
#define SEG_32		0x40		/* Is 32 bit? 			*/
#define SEG_GBYTE	0x00		/* Granularity byte?	*/
#define SEG_GPAGE	0x80		/* Granularity page?	*/

#define SEG_KERN	0x00		/* Kernel permissions? 	*/
#define SEG_USER	0x60		/* User permissions? 	*/


void set_tss(w_uint32 esp){

    current_tss.esp0 = esp;
    current_tss.cs   = 0x08; // 0x0b for user mode
    current_tss.ss = current_tss.ds = current_tss.es = 0x10; 
	current_tss.fs = current_tss.gs = 0x10; // 0x13 for user mode ^
}

/* Initialize segments */

static void seg_init(){

	g_ptr.limit = (sizeof(struct w_gdte) * 5) - 1;
	g_ptr.base  = (w_uint32)&gdt_entries;

	w_uint32 tmp_access;
	w_uint32 tmp_gran;

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
	w_uint32 addr = (w_uint32)&current_tss;

	set_gdt(6, addr, sizeof(struct w_tss), tmp_access, tmp_gran);


	gdt_flush(&g_ptr);
}

static void idt_init(){

	i_ptr.limit = sizeof(struct w_idte) * 256 -1;
	i_ptr.base  = (w_uint32)&idt_entries;

	zero_mem(&idt_entries, sizeof(struct w_idte) * 256);

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

	set_idt( 0, (w_uint32)isr0 , SEG_KERNEL_CODE, 0x8E);
	set_idt( 1, (w_uint32)isr1 , SEG_KERNEL_CODE, 0x8E);
	set_idt( 2, (w_uint32)isr2 , SEG_KERNEL_CODE, 0x8E);
	set_idt( 3, (w_uint32)isr3 , SEG_KERNEL_CODE, 0x8E);
	set_idt( 4, (w_uint32)isr4 , SEG_KERNEL_CODE, 0x8E);
	set_idt( 5, (w_uint32)isr5 , SEG_KERNEL_CODE, 0x8E);
	set_idt( 6, (w_uint32)isr6 , SEG_KERNEL_CODE, 0x8E);
	set_idt( 7, (w_uint32)isr7 , SEG_KERNEL_CODE, 0x8E);
	set_idt( 8, (w_uint32)isr8 , SEG_KERNEL_CODE, 0x8E);
	set_idt( 9, (w_uint32)isr9 , SEG_KERNEL_CODE, 0x8E);
	set_idt(10, (w_uint32)isr10, SEG_KERNEL_CODE, 0x8E);
	set_idt(11, (w_uint32)isr11, SEG_KERNEL_CODE, 0x8E);
	set_idt(12, (w_uint32)isr12, SEG_KERNEL_CODE, 0x8E);
	set_idt(13, (w_uint32)isr13, SEG_KERNEL_CODE, 0x8E);
	set_idt(14, (w_uint32)isr14, SEG_KERNEL_CODE, 0x8E);
	set_idt(15, (w_uint32)isr15, SEG_KERNEL_CODE, 0x8E);
	set_idt(16, (w_uint32)isr16, SEG_KERNEL_CODE, 0x8E);
	set_idt(17, (w_uint32)isr17, SEG_KERNEL_CODE, 0x8E);
	set_idt(18, (w_uint32)isr18, SEG_KERNEL_CODE, 0x8E);
	set_idt(19, (w_uint32)isr19, SEG_KERNEL_CODE, 0x8E);
	set_idt(20, (w_uint32)isr20, SEG_KERNEL_CODE, 0x8E);
	set_idt(21, (w_uint32)isr21, SEG_KERNEL_CODE, 0x8E);
	set_idt(22, (w_uint32)isr22, SEG_KERNEL_CODE, 0x8E);
	set_idt(23, (w_uint32)isr23, SEG_KERNEL_CODE, 0x8E);
	set_idt(24, (w_uint32)isr24, SEG_KERNEL_CODE, 0x8E);
	set_idt(25, (w_uint32)isr25, SEG_KERNEL_CODE, 0x8E);
	set_idt(26, (w_uint32)isr26, SEG_KERNEL_CODE, 0x8E);
	set_idt(27, (w_uint32)isr27, SEG_KERNEL_CODE, 0x8E);
	set_idt(28, (w_uint32)isr28, SEG_KERNEL_CODE, 0x8E);
	set_idt(29, (w_uint32)isr29, SEG_KERNEL_CODE, 0x8E);
	set_idt(30, (w_uint32)isr30, SEG_KERNEL_CODE, 0x8E);
	set_idt(31, (w_uint32)isr31, SEG_KERNEL_CODE, 0x8E);
	set_idt(32, (w_uint32)irq0, SEG_KERNEL_CODE, 0x8E);
	set_idt(33, (w_uint32)irq1, SEG_KERNEL_CODE, 0x8E);
	set_idt(34, (w_uint32)irq2, SEG_KERNEL_CODE, 0x8E);
	set_idt(35, (w_uint32)irq3, SEG_KERNEL_CODE, 0x8E);
	set_idt(36, (w_uint32)irq4, SEG_KERNEL_CODE, 0x8E);
	set_idt(37, (w_uint32)irq5, SEG_KERNEL_CODE, 0x8E);
	set_idt(38, (w_uint32)irq6, SEG_KERNEL_CODE, 0x8E);
	set_idt(39, (w_uint32)irq7, SEG_KERNEL_CODE, 0x8E);
	set_idt(40, (w_uint32)irq8, SEG_KERNEL_CODE, 0x8E);
	set_idt(41, (w_uint32)irq9, SEG_KERNEL_CODE, 0x8E);
	set_idt(42, (w_uint32)irq10, SEG_KERNEL_CODE, 0x8E);
	set_idt(43, (w_uint32)irq11, SEG_KERNEL_CODE, 0x8E);
	set_idt(44, (w_uint32)irq12, SEG_KERNEL_CODE, 0x8E);
	set_idt(45, (w_uint32)irq13, SEG_KERNEL_CODE, 0x8E);
	set_idt(46, (w_uint32)irq14, SEG_KERNEL_CODE, 0x8E);
	set_idt(47, (w_uint32)irq15, SEG_KERNEL_CODE, 0x8E);

	idt_flush(&i_ptr);
}

void init_seg(){

	seg_init();
	idt_init();
	zero_mem(&interrupt_handlers, sizeof(w_isr) * 256);
	//tss_flush();
}

/* Set the value of one GDT entry */

static void set_gdt(int n,w_uint32 base,w_uint32 lim,w_uint8 a,w_uint8 gran){

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

static void set_idt(w_uint8 num, w_uint32 base, w_uint16 sel, w_uint8 flags){

    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel     = sel;
    idt_entries[num].always0 = 0;

    /* OR with 0x60 for user mode for privilege level 3 */

    idt_entries[num].flags   = flags /* | 0x60 */;
}
