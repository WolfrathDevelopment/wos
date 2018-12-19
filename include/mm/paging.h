#include <mm/mem.h>
#include <types.h>
#include <lib/core.h>

#ifndef _OS_PAGING_H
#define _OS_PAGING_H

/* Paging sizes */

#define PDE_MAX_ENTRIES     (0x400)
#define PTE_MAX_ENTRIES     (0x400)
#define PAGE_SIZE           (0x1000)
#define MAX_MEM_SIZE        (0x4000000) // 64 MB for now

/* Page directory and table index */

#define PDE_INDEX(_va)  ((typeof(_va)) (((uint32_t)_va) >> 22))
#define PTE_INDEX(_va)  ((typeof(_va)) ((((uint32_t)_va) >> 22) & 0x3FF))

#define FLAG_ALIGN      (0x1)
#define FLAG_NOALIGN	(0x0)

#define CR0_PAGING_ENABLED  (0x80000000)

#define PAGE_FAULT_ERR_EXIST        (0x01)
#define PAGE_FAULT_ERR_WRITE_OP     (0x02)
#define PAGE_FAULT_ERR_USER_MODE    (0x04)
#define PAGE_FAULT_ERR_RESERVED     (0x08)
#define PAGE_FAULT_ERR_FETCH        (0x10)

typedef uint32_t    PageFrameIndex;
#define PageFrameIndexInvalid   ((PageFrameIndex)0)

typedef union
{
    struct
    {
        uint32_t present        : 1;    // Is the table physically there?
        uint32_t writable       : 1;    // Can we write to the table?
        uint32_t user           : 1;    // Can user level code access the table?
        uint32_t accessed       : 1;    // Has the page been accessed recently?
        uint32_t dirty          : 1;    // Has the page been written to recently?
        uint32_t unused         : 7;    //
        uint32_t table_frame    : 20;   // Page table index
	};
	uint32_t all_fields;
} PageDirEntry;

typedef union
{
    struct
    {
        uint32_t present    : 1;    // Is the page physically there?
        uint32_t writable   : 1;    // Can we write to the page?
        uint32_t user       : 1;    // Can user level code access the page?
        uint32_t accessed   : 1;    // Has the page been accessed recently?
        uint32_t dirty      : 1;    // Has the page been written to recently?
        uint32_t unused     : 7;    //
        uint32_t page_frame : 20;   // Page frame index
	};
	uint32_t all_fields;
} PageTblEntry;

struct _PageTable
{
    PageTblEntry pages[PDE_MAX_ENTRIES];
};
typedef struct _PageTable PageTable;

struct _PageDirectory
{
    PageDirEntry tables[PDE_MAX_ENTRIES];
};
typedef struct _PageDirectory PageDirectory;

int32_t page_is_mapped(PageDirectory* dir, uint32_t virt_address);
void    page_map_virtual(PageDirectory* dir, uint32_t virt_address, PageTblEntry pte);
void    page_unmap(PageDirectory* dir, uint32_t virt_address);

/* Invalidate a TLB entry */
static inline void invlpg(uint32_t virt_address)
{
    asm volatile("invlpg (%0)" ::"r" (virt_address) : "memory");
}

#endif /* __WOS_PAGING_H */
