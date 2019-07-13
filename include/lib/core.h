/*
 * core.h
 * Joel Wolfrath, 2013
 *
 * Basic io functions and string manipulation
 */

#ifndef CORE_H
#define CORE_H

#include <types.h>

# define ALIGN(value, boundary) \
	( (typeof(value)) ((((uint32_t)value) + (boundary-1)) & ~(boundary-1)))

inline uint32_t read_eflags(void)
{
	uint32_t eflags = 0;
	asm volatile("pushfl; popl %0" : "=r" (eflags));
	return eflags;
}

static inline void cli(void)
{
	asm volatile("cli");
}

static inline void sti(void)
{
	asm volatile("sti");
}

#endif
