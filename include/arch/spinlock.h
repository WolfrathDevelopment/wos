/*
 * spinlock.h
 * Joel Wolfrath, 2013
 *
 * Spinlock definitions
 */

#include <types.h>
#include <arch/cpu.h>
#include <arch/regs.h>
#include <tools/debug.h>

#ifndef _OS_SPINLOCK_H
#define _OS_SPINLOCK_H

#define SPINLOCK_INIT(l) \
    (l).value = 0; \
    (l).owner = CPU_MAX

typedef struct
{
    uint32_t value;
    uint32_t owner;
} Spinlock;

extern void _lock(uint32_t * value);
extern void _unlock(uint32_t * value);

static inline void lock(Spinlock * spinlock)
{
    if(spinlock->owner == CPU_DEFAULT_ID)
    {
        PANIC("We already hold the lock!");
    }

    _lock(&spinlock->value);
    spinlock->owner = CPU_DEFAULT_ID;
}

static inline void unlock(Spinlock * spinlock)
{
    if(spinlock->owner != CPU_DEFAULT_ID)
    {
        PANIC("Unlocking a lock we don't hold!");
    }

    spinlock->owner = CPU_MAX;
    _unlock(&spinlock->value);
}

#endif /* _OS_SPINLOCK_H */
