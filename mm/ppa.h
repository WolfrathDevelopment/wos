#ifndef WOS_PPA_H
#define WOS_PPA_H

#include "../boot/multiboot.h"
#include "debug.h"
#include "mem.h"
#include "paging.h"

/* This will come out evenly provied sane values of MAX_MEM_SIZE */
#define MAX_PPA_ENTRIES ((MAX_MEM_SIZE / PAGE_SIZE) / 32)

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

	// spinlock TODO

	uint32 bitmap[ MAX_PPA_ENTRIES ];

} PhysicalPageAllocator;


/* Our global instance */
extern PhysicalPageAllocator PPAInstance;

/* Use memory map from the bootloader to initialize our structures */
void ppa_init(GrubMultibootInfo *);

Page alloc_page();
void free_page(Page currentPage);

#endif
