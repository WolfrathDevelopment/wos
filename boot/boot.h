/*
 * boot.h
 * Wolfrath/Kriewall, 2013
 *
 * Functions needed to modify:
 * 	IDT
 * 	GDT
 *	ISR Functionality
 *
 */

#ifndef BOOT_H
#define BOOT_H

#include "../core/core.h"

#define INT_PAGEFAULT 	14
#define INT_PIC		32
#define IRQ1		33
#define IRQ2 		34
#define IRQ3 		35
#define IRQ4 		36
#define IRQ5 		37
#define IRQ6 		38
#define IRQ7 		39
#define IRQ8 		40
#define IRQ9 		41
#define IRQ10 		42
#define IRQ11 		43
#define IRQ12 		44
#define IRQ13 		45
#define IRQ14 		46
#define IRQ15 		47

#define SEG_KERNEL_CODE		0x08
#define SEG_KERNEL_DATA		0x10
#define SEG_USER_CODE		0x18
#define SEG_USER_DATA		0x20
#define SEG_USER_TLS		0x28

#define REGS_COPY(r1,r2) \
            (r1)->ds = (r2)->ds; \
            (r1)->edi = (r2)->edi; \
            (r1)->esi = (r2)->esi; \
            (r1)->ebp = (r2)->ebp; \
            (r1)->esp = (r2)->esp; \
            (r1)->ebx = (r2)->ebx; \
            (r1)->edx = (r2)->edx; \
            (r1)->ecx = (r2)->ecx; \
            (r1)->eax = (r2)->eax; \
            (r1)->int_no = (r2)->int_no; \
            (r1)->err_code = (r2)->err_code; \
            (r1)->eip = (r2)->eip; \
            (r1)->cs = (r2)->cs; \
            (r1)->eflags = (r2)->eflags; \
            (r1)->useresp = (r2)->useresp; \
            (r1)->ss = (r2)->ss

void init_seg();
void init_idt();

/* ASM flush routines */

extern void gdt_flush(uint);
extern void idt_flush(uint);
extern void tss_flush();


void set_tss(uint);

/* Models the task state segment */

struct w_tss{

	uint prev_tss;
	uint esp0;
	uint ss0;
	uint esp1;
	uint ss1;
	uint esp2;
	uint ss2;
	uint cr3;
	uint eip;
	uint eflags;
	uint eax;
	uint ecx;
	uint edx;
	uint ebx;
	uint esp;
	uint ebp;
	uint esi;
	uint edi;
	uint es;
	uint cs;
	uint ss;
	uint ds;
	uint fs;
	uint gs;
	uint ldt;
	ushort trap;
	ushort iomap_base;
} __attribute__((packed));

struct w_gdte{

    ushort limit_low;		// The lower 16 bits of the limit.
    ushort base_low;    	// The lower 16 bits of the base.
    uchar  base_middle; 	// The next 8 bits of the base.
    uchar  access;  		// Access ring
    uchar  granularity;
    uchar  base_high;		// The last 8 bits of the base.
} __attribute__((packed));

/*
 * Defines a GDT pointer.
 * Points to beginning of array of w_gdte
 */
struct w_gdtp{

    ushort limit;		// The upper 16 bits of limit
    uint base;			// The address of the first w_gdte
} __attribute__((packed));

/* A struct describing an interrupt gate */
struct w_idte{

    ushort base_lo;		// The lower 16 of jmp address
    ushort sel;			// Kernel segment selector.
    uchar  always0;		// This must always be zero.
    uchar  flags;		// More flags.
    ushort base_hi;		// The upper 16 of jmp address
} __attribute__((packed));

/* pointer to an array of interrupt handlers */
struct w_idtp{

    ushort limit;
    uint base;			// The address of the first w_idte
} __attribute__((packed));

/* ISR handlers */

extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void irq0 ();
extern void irq1 ();
extern void irq2 ();
extern void irq3 ();
extern void irq4 ();
extern void irq5 ();
extern void irq6 ();
extern void irq7 ();
extern void irq8 ();
extern void irq9 ();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

/* Models x86 registers */
struct w_regs{

	/* Data segment selector */
	uint ds;

	/* Registers pushed from call to pusha */
	uint edi, esi, ebp, esp, ebx, edx, ecx, eax;

	/* Interrupt number and error */
	uint int_no, err_code;

	/* Processor flags */
	uint eip, cs, eflags, useresp, ss;
};

/* Generic ISR callback function */
typedef void (*w_isr)(struct w_regs);
void register_interrupt_handler(uchar, w_isr);

#endif
