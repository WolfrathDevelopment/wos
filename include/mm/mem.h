/*
 * mem.h
 * Joel Wolfrath, 2013
 *
 * Defs for the x86 MMU
 * Management for physical memory
 */

#ifndef MEM_H
#define MEM_H

#include <arch/regs.h>
#include <boot/boot.h>
#include <boot/multiboot.h>
#include <lib/core.h>
#include <tools/debug.h>
#include <lib/list.h>
#include <mm/paging.h>

/* Linker provides these values */
extern uint32 kern_start;
extern uint32 kern_end;

/* Deep magic begins here... */

/*
	USER VIRTUAL MEMORY LAYOUT (ELF)
	( * may expand to accomodate larger files )

	----------------------------------------------0xFFFFFFFF
	|	KERNEL SPACE ( 1 GB )
	----------------------------------------------0xC0000000
	|	STACK (grows down)
	--------------------------------------------------------

	--------------------------------------------------------
	|	MMAP REGION (grows up)
	----------------------------------------------0x40000000

	--------------------------------------------------------
	|	HEAP (grows up)
	----------------------------------------------0x00003000
	|	BSS SEGMENT *
	----------------------------------------------0x00002000
	|	DATA SEGMENT *
	----------------------------------------------0x00001000
	|	TEXT SEGMENT (ELF) *
	----------------------------------------------0x00000000
*/

/*
	KERNEL VIRTUAL MEMORY
	----------------------------------------------0xFFFFFFFF
	|	DIRECT MAPPED DEVICES (APIC)
	----------------------------------------------0xFE000000
	|	STACK (grows down)
	--------------------------------------------------------
	|
	--------------------------------------------------------
	|	STACK GUARD PAGE
	----------------------------------------------0xFDFFC000

	--------------------------------------------------------
	|	HEAP (grows up)
	----------------------------------------------- + kern_end
	|	BSS SEGMENT
	--------------------------------------------------------
	|	DATA SEGMENT
	--------------------------------------------------------
	|	TEXT SEGMENT (ELF)
	---------------------------------------------  + kern_start
	|	DMA area
	-----------------------------------------------0xC0000000

*/

/* Kernel reserved addresses */

#define KMAP_START			0xC0000000
#define KSTACK_BASE			0xFE000000
#define KSTACK_GRD			(KSTACK_BASE - 0x4000)

#define PD_INDEX(va)			(va >> 22)
#define PT_INDEX(va)			((va >> 12) & 0x3FF)

/* Kernel mapping macros */

#define KPHYS(x)			(x - KMAP_START)
#define KVIRT(x)			(x + KMAP_START)

typedef uint32	PageDirectoryEntry;
typedef uint32	PageTableEntry;

enum PTE_FLAGS {

    PTE_Present         = 0x0001,
    PTE_Writable        = 0x0002,
    PTE_User            = 0x0004,
    PTE_WriteThrough    = 0x0008,
    PTE_CacheDisabled   = 0x0010,
    PTE_Accessed        = 0x0020,
    PTE_Dirty           = 0x0040,
    PTE_PageSize        = 0x0080,
    PTE_ZeroBit         = 0x0100,
    PTE_Ignored         = 0x0200,
    PTE_Available1      = 0x0400,
    PTE_Available2      = 0x0800
};

/* mmap.c*/

typedef struct {

    uint32_t size;
    uint32_t base_addr_low,base_addr_high;
    uint32_t length_low,length_high;
    uint32_t type;

} GrubMemoryMapEntry;

void* read_mmap(GrubMultibootInfo* mbt);
void print_mmap_entry(GrubMemoryMapEntry*);
void map_kernel();
void* kmalloc(uint32 size, int32 align);


/* paging.c */

void set_page_directory(PageDirectoryEntry*);
void page_fault_handler(OsIsrFrame*);
void map_page(PageDirectoryEntry*,uint32, PageTableEntry);
int is_mapped(PageDirectoryEntry*, uint32);
void unmap_page(PageDirectoryEntry*,uint32);
void init_paging();
void invalidate_page(uint32);


/* alloc.c */

void init_alloc(GrubMultibootInfo*);
PageTableEntry alloc_page_frame(uint32);
void free_page_frame(uint32);


/* kheap.c */

#define INIT_HEAP_SIZE 0x4000

struct w_block{

	uint32 start;
	uint32 end;
	LINKED_LIST_MEMBER(block_node);
};

struct w_heap{

	uint32 start_addr;
	uint32 size;
	LINKED_LIST(block_list_head);
};

void init_kheap();
void* kalloc(uint32);
void kfree(uint32);
void brk(struct w_heap*);

#endif
