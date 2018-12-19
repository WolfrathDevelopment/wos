/*
 * debug.h
 * Joel Wolfrath, 2013
 *
 * Standard debugging functions
 */

#ifndef _OS_DEBUG_H
#define _OS_DEBUG_H

#include <types.h>
#include <arch/regs.h>
#include <lib/core.h>

void _die(OsBaseRegs* regs, char* file, uint32_t line);
void _debug(char * msg, char * file, uint32_t line);
void _panic(char * msg, char* file, uint32_t line);
void _assert(int32_t value, char* file, uint32_t line);
void _trace_rc(OsRc rc, char* file, uint32_t line);

void trace_stack(uint32_t);
void dump_memory(uint8_t* address, uint32_t length);

FORCE_INLINE static void die(char* file, uint32_t line)
{
    asm("pusha");
    OsBaseRegs* regs = (OsBaseRegs*)_esp();
    _die(regs, file, line);
};

#define DEBUG(x)    _debug(x, __FILE__, __LINE__)
#define PANIC(x)    _panic(x, __FILE__, __LINE__)
//#define ASSERT(x)   _assert(((int32_t)x), __FILE__, __LINE__)
#define ASSERT(x)   if(!(x)) { die(__FILE__, __LINE__); }
#define TRACE_RC(x) _trace_rc(x, __FILE__, __LINE__)

FORCE_INLINE void dump_registers()
{
    
};

#endif /* _OS_DEBUG_H */
