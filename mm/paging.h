#ifndef __WOS_PAGING_H
#define __WOS_PAGING_H

#include "mem.h"
#include "../types.h"
#include "../core/core.h"

/* Paging sizes */

#define PDE_MAX_ENTRIES		(0x400)
#define PTE_MAX_ENTRIES		(0x400)
#define PAGE_SIZE			(0x1000)
#define MAX_MEM_SIZE		(0x2000000) // 32 MB for now

/* Page directory and table index */

static inline int getPDEIndex(void * virtualAddress){
	return ((uint32)virtualAddress) >> 22;
}

static inline int getPTEIndex(void * virtualAddress){
	return (((uint32)virtualAddress) >> 22) & 0x3FF;
}

/* Get address/flags out of page table or directory */

#define PTE_ADDR(pte)			(pte & ~0xFFF)
#define PTE_FLAGS(pte)			(pte & 0xFFF)

#define FLAG_ALIGN			    (0x1)
#define FLAG_NOALIGN			(0x0)

typedef uint32	PageDirectoryEntry;
typedef uint32	PageTableEntry;

typedef union
{
	struct
	{
		uint32 present	: 1;	// Is the page physically there?
		uint32 writable	: 1;	// Can we write to the page?
		uint32 user		: 1;	// Can user level code access the page?
		uint32 accessed	: 1;	// Has the page been accessed recently?
		uint32 dirty	: 1;	// Has the page been written to recently?
		uint32 unused	: 7;	//
		uint32 frame	: 20;	// Page frame index
	};
	uint32 all_fields;
} Page;

static inline int isPagePresent(uint32 pte){
    return pte & 0x0001; //PTE_Present;
}

static inline int isPtePresent(uint32 pde){
    return pde & 0x0001; //PTE_Present;
}

#endif /* __WOS_PAGING_H */
