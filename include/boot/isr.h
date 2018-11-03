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
    
    MaxIsrId     = 256

} InterruptId;

/* Function signature for an ISR */
typedef void (*InterruptServiceRoutine)(Registers regs);

/* Setup the interrupt descriptor table */
OsRc init_idt();

OsRc register_isr(InterruptId id, InterruptServiceRoutine routine);

#endif /* _OS_ISR_H */
