/*
 * boot.h
 * Joel Wolfrath, 2013
 *
 * Functions needed to modify:
 * 	IDT
 * 	GDT
 *	ISR Functionality
 */

#ifndef BOOT_H
#define BOOT_H

#include <lib/core.h>
#include <lib/string.h>

#define INT_PAGEFAULT 		(14)
#define INT_PIC			(32)
#define IRQ1			(33)
#define IRQ2	 		(34)
#define IRQ3	 		(35)
#define IRQ4	 		(36)
#define IRQ5	 		(37)
#define IRQ6	 		(38)
#define IRQ7	 		(39)
#define IRQ8	 		(40)
#define IRQ9	 		(41)
#define IRQ10	 		(42)
#define IRQ11	 		(43)
#define IRQ12 			(44)
#define IRQ13 			(45)
#define IRQ14 			(46)
#define IRQ15 			(47)

#define SEG_KERNEL_CODE		(0x08)
#define SEG_KERNEL_DATA		(0x10)
#define SEG_USER_CODE		(0x18)
#define SEG_USER_DATA		(0x20)
#define SEG_USER_TLS		(0x28)

/* Lets skip the overhead for copying a process's registers */

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

void set_tss(uint32);

/* Models the task state segment */

typedef struct {

	uint32 prev_tss;
	uint32 esp0;
	uint32 ss0;
	uint32 esp1;
	uint32 ss1;
	uint32 esp2;
	uint32 ss2;
	uint32 cr3;
	uint32 eip;
	uint32 eflags;
	uint32 eax;
	uint32 ecx;
	uint32 edx;
	uint32 ebx;
	uint32 esp;
	uint32 ebp;
	uint32 esi;
	uint32 edi;
	uint32 es;
	uint32 cs;
	uint32 ss;
	uint32 ds;
	uint32 fs;
	uint32 gs;
	uint32 ldt;
	uint16 trap;
	uint16 iomap_base;

} __attribute__((packed)) TaskStateSegment;

typedef struct {

    uint16 limit_low;
    uint16 base_low;
    uint8  base_middle;
    uint8  access;
    uint8  granularity;
    uint8  base_high;

} __attribute__((packed)) GlobalDescTableEntry;


/*
 * Defines a GDT pointer.
 * Points to beginning of array of w_gdte
 */

typedef struct {

    uint16 limit;
    uint32 base;		/* Address of the first w_gdte */

} __attribute__((packed)) GlobalDescTablePointer;


/* A struct describing an interrupt gate */

typedef struct {

    uint16 base_lo;		/* lower 16 bits of jmp address */
    uint16 sel;			/* Kernel segment selector */
    uint8  always0;		
    uint8  flags;
    uint16 base_hi;		/* upper 16 bits of jmp address */

} __attribute__((packed)) InterruptDescTableEntry;


/* pointer to an array of interrupt handlers */

typedef struct {

    uint16 limit;
    uint32 base;		/* Address of the first w_idte */

} __attribute__((packed)) InterruptDescTablePointer;

/* ASM flush routines */

extern void gdt_flush(GlobalDescTablePointer*) ;
extern void idt_flush(InterruptDescTablePointer*);
extern void tss_flush();

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

typedef struct {

	/* Data segment selector */

	uint32 ds;

	/* Registers pushed from call to pusha */

	uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax;

	/* Interrupt number and error */

	uint32 int_no, err_code;

	/* Processor flags */

	uint32 eip, cs, eflags, useresp, ss;

} Registers;

/* Generic ISR callback function */

typedef void (*InterruptServiceRoutine)(Registers);

void register_isr(uint8 index, InterruptServiceRoutine routine);

#endif
