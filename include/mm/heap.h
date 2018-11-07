#include <mm/mem.h>
#include <lib/atomic.h>

#ifndef _OS_HEAP_H
#define _OS_HEAP_H

#define HEAP_MAGIC  (0x12345678)

typedef struct {

    uint32_t    magic;
    
} AllocMetadata;

typedef struct {

    Spinlock lock;
    
} Heap;

#endif /* _OS_HEAP_H */
