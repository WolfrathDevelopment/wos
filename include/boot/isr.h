/*
 * isr.h
 * Joel Wolfrath, 2013
 *
 * Defines for interrupt service routines
 */

#include <arch/regs.h>
#include <types.h>

#ifndef _OS_ISR_H
#define _OS_ISR_H

typedef enum {
    
    OsIsrPageFault  = 0x00D,
    OsIsrPic        = 0x020,
    OsIsrIrq1       = 0x021,
    OsIsrMaxId      = 0x100

} OsIsrId;

/* Function signature for an ISR */
typedef void (*OsIsr)(OsIsrFrame* frame);

/* Setup the interrupt descriptor table */
OsRc idt_init();

OsRc isr_register(OsIsrId id, OsIsr routine);

#endif /* _OS_ISR_H */
