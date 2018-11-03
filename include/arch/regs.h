/*
 * regs.h
 * Joel Wolfrath, 2013
 *
 * Defines register stack frame 
 */

#include <types.h>

#ifndef _OS_REGS_H
#define _OS_REGS_H

/* Models x86 registers */

typedef struct {

    /* Data segment selector */

    uint32_t ds;

    /* Registers pushed from call to pusha */

    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;

    /* Interrupt number and error */

    uint32_t int_no, err_code;

    /* Processor flags */

    uint32_t eip, cs, eflags, useresp, ss;

} Registers;

#endif /* _OS_REGS_H */
