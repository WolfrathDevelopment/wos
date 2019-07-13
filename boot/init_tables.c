/*
 * init_talbes.c
 * Joel Wolfrath, 2013
 *
 * Initializes...
 * 	GDT
 *	IDT
 *	ISR/IRQ Handlers
 */

#include <boot/boot.h>
#include <boot/isr.h>

static void set_gdt(int, uint32_t, uint32_t, uint8_t, uint8_t);
static void set_idt(uint8_t, uint32_t, uint16_t, uint8_t);

/* The new GDT */

GlobalDescTableEntry    gdt_entries[7];
GlobalDescTablePointer  g_ptr;


/* Task state of current task */

TaskStateSegment current_tss;

/* Segment flags */

#define SEG_P     	(0x80)		/* Is Present? 		*/
#define SEG_TSS		(0x01)		/* Is TSS? 		*/
#define SEG_SEG		(0x10)		/* Is Segment? 		*/
#define SEG_X		(0x08)		/* Executable? 		*/
#define SEG_W		(0x02)		/* Writable? 		*/
	
#define SEG_AVAIL	(0x10)		/* Is Available? 	*/
#define SEG_32		(0x40)		/* Is 32 bit? 		*/
#define SEG_GBYTE	(0x00)		/* Granularity byte?	*/
#define SEG_GPAGE	(0x80)		/* Granularity page?	*/

#define SEG_KERN	(0x00)		/* Kernel permissions? 	*/
#define SEG_USER	(0x60)		/* User permissions? 	*/


void set_tss(uint32_t esp){

	current_tss.esp0 = esp;
	current_tss.cs   = 0x08; // 0x0b for user mode
	current_tss.ss = current_tss.ds = current_tss.es = 0x10; 
	current_tss.fs = current_tss.gs = 0x10; // 0x13 for user mode ^
}

/* Initialize segments */

static void seg_init(){

	g_ptr.limit = (sizeof(GlobalDescTableEntry) * 5) - 1;
	g_ptr.base  = (uint32_t)&gdt_entries;

	uint32_t tmp_access;
	uint32_t tmp_gran;

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
	uint32_t addr = (uint32_t)&current_tss;

	set_gdt(6, addr, sizeof(TaskStateSegment), tmp_access, tmp_gran);

	gdt_flush(&g_ptr);
}

void init_seg(){

	seg_init();
	//idt_init();
	//tss_flush();
}

/* Set the value of one GDT entry */

static void set_gdt(int n,uint32_t base, uint32_t lim, uint8_t a, uint8_t gran){

	gdt_entries[n].base_low = (base & 0xFFFF);
	gdt_entries[n].base_middle = (base >> 16) & 0xFF;
	gdt_entries[n].base_high = (base >> 24) & 0xFF;

	gdt_entries[n].limit_low = (lim & 0xFFFF);
	gdt_entries[n].granularity = (lim >> 16) & 0x0F;

	gdt_entries[n].granularity |= gran & 0xF0;
	gdt_entries[n].access = a;
}

