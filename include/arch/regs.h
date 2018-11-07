/*
 * regs.h
 * Joel Wolfrath, 2013
 *
 * Register definitions
 */

#include <types.h>

#ifndef _OS_REGS_H
#define _OS_REGS_H

/* Models x86 registers */

typedef struct
{
    /* Data segment selector */

    uint32_t ds;

    /* Registers pushed from call to pusha */

    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;

    /* Interrupt number and error */

    uint32_t int_no, err_code;

    /* Processor flags */

    uint32_t eip, cs, eflags, useresp, ss;

} Registers;

/* Caller instruction pointer is 4 bytes above ebp */
FORCE_INLINE static inline uint32_t get_caller_eip()
{
    register uint32_t ebp asm("ebp");
    return *((uint32_t*)(ebp + 4));
}

#endif /* _OS_REGS_H */
