
// wOS - Joel Wolfrath, 2013
// boot.h - Defines the functions needed to modify
// the IDT and GDT and functionality for isr's.

#ifndef BOOT_H
#define BOOT_H

#include "../types.h"

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

void init_descriptor_tables();

// Defines a GDT entry
struct gdt_entry_struct{

    ushort limit_low;           // The lower 16 bits of the limit.
    ushort base_low;            // The lower 16 bits of the base.
    uchar  base_middle;         // The next 8 bits of the base.
    uchar  access;              // Access flags, determine what ring this segment can be used in.
    uchar  granularity;
    uchar  base_high;           // The last 8 bits of the base.
} __attribute__((packed));

typedef struct gdt_entry_struct GDT_e;

// Defines a GDT pointer.
// Points to beginning of array of GDT_e
struct gdt_ptr_struct{

    ushort limit;               // The upper 16 bits of all selector limits.
    uint base;                // The address of the first gdt_entry struct.
} __attribute__((packed));

typedef struct gdt_ptr_struct GDT_p;

// A struct describing an interrupt gate.
struct idt_entry_struct{

    ushort base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
    ushort sel;                 // Kernel segment selector.
    uchar  always0;             // This must always be zero.
    uchar  flags;               // More flags.
    ushort base_hi;             // The upper 16 bits of the address to jump to.
} __attribute__((packed));

typedef struct idt_entry_struct IDT_e;

// pointer to an array of interrupt handlers.
struct idt_ptr_struct{

    ushort limit;
    uint base;                // The address of the first element in the idt_entry array.
} __attribute__((packed));

typedef struct idt_ptr_struct IDT_p;

// The addresses of the ASM ISR handlers.
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

typedef struct{

    uint ds;                  // Data segment selector
    uint edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    uint int_no, err_code;    // Interrupt number and error code
    ulint eip, cs, eflags, useresp, ss; // Pushed by the processor
} Registers;

// Generic callback
typedef void (*w_isr)(Registers);
void register_interrupt_handler(uchar, w_isr);

#endif
