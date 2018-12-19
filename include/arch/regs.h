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
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;

} OsBaseRegs;

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

} OsIsrFrame;

FORCE_INLINE static uint32_t _ebp()
{
    register uint32_t ebp asm("ebp");
    return ebp;
}

FORCE_INLINE static uint32_t _esp()
{
    register uint32_t esp asm("esp");
    return esp;
}

/* Caller instruction pointer is 4 bytes above ebp */
FORCE_INLINE static uint32_t get_caller_eip()
{
    register uint32_t ebp asm("ebp");
    return *((uint32_t*)(ebp + 4));
}

FORCE_INLINE static uint32_t _cr0()
{
    uint32_t cr0 = 0;
    asm volatile("mov %%cr0, %0" : "=r" (cr0));
    return cr0;
}

FORCE_INLINE static uint32_t _cr2()
{
    uint32_t cr2 = 0;
    asm volatile("mov %%cr2, %0" : "=r" (cr2));
    return cr2;
}

#endif /* _OS_REGS_H */
