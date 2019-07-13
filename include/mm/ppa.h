#ifndef _OS_PPA_H
#define _OS_PPA_H

#include <arch/spinlock.h>
#include <boot/multiboot.h>
#include <tools/debug.h>
#include <mm/mem.h>
#include <mm/paging.h>

/* This will come out evenly provied sane values of MAX_MEM_SIZE */
#define MAX_PPA_ENTRIES ((MAX_MEM_SIZE / PAGE_SIZE) / sizeof(uint32_t))

/*
 * This is a singleton class responsible for all the allocating and 
 * freeing of physical pages. Once allocated, then these pages can
 * be mapped to a page table for use by the kernel or user process.
 *
 * For now, use a single bit for each page to indicate its state.
 * 0 -> page in use
 * 1 -> page available
 */

typedef struct {

    Spinlock lock;
    uint32_t bitmap[ MAX_PPA_ENTRIES ];

} PhysicalPageAllocator;


/* Our global instance */
extern PhysicalPageAllocator PPAInstance;

/* Use memory map from the bootloader to initialize our structures */
void ppa_init(GrubMultibootInfo *);

PageFrameIndex alloc_page();
void free_page(PageFrameIndex frame);

#endif /* _OS_PPA_H */
