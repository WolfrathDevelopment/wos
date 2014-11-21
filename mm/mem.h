/*
 * mem.h
 * Joel Wolfrath, 2013
 *
 * Defs for the x86 MMU
 * Management for physical memory
 */

#ifndef MEM_H
#define MEM_H

#include "../boot/boot.h"
#include "../boot/multiboot.h"
#include "../core/core.h"
#include "../util/debug.h"
#include "../lib/list.h"

extern w_uint32 kern_start;
extern w_uint32 kern_end;

/* Deep magic begins here... */

/*
	USER VIRTUAL MEMORY LAYOUT (ELF)
	( * may expand to accomodate larger files )

	----------------------------------------------0xFFFFFFFF
	|	KERNEL SPACE ( 1 GB )
	----------------------------------------------0xC0000000
	|	STACK (grows down)
	------------------------------------------------------------

	------------------------------------------------------------
	|	MMAP REGION (grows up)
	----------------------------------------------0x40000000

	------------------------------------------------------------
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
	-------------------------------------------------------------
	|
	-------------------------------------------------------------
	|	STACK GUARD PAGE
	----------------------------------------------0xFDFFC000

	-------------------------------------------------------------
	|	HEAP (grows up)
	----------------------------------------------- + kern_end
	|	BSS SEGMENT
	-------------------------------------------------------------
	|	DATA SEGMENT
	-------------------------------------------------------------
	|	TEXT SEGMENT (ELF)
	---------------------------------------------  + kern_start
	|	DMA area
	-----------------------------------------------0xC0000000

*/

/* Kernel reserved addresses */

#define KMAP_START			0xC0000000
#define KSTACK_BASE			0xFE000000
#define KSTACK_GRD			(KSTACK_BASE - 0x4000)


/* Kernel mapping macros */

#define KPHYS(x)			(x - KMAP_START)
#define KVIRT(x)			(x + KMAP_START)


/* Size of page directories and page tables */

#define NUM_PDE				0x400
#define NUM_PTE				0x400
#define PAGE_SIZE			0x1000


/* Page directory and table index */

#define PD_INDEX(va)			(va >> 22)
#define PT_INDEX(va)			((va >> 12) & 0x3FF)


/* Page table/directory flags */

#define PTE_P				0x001		/* present */
#define PTE_W				0x002 		/* writeable */
#define PTE_U				0x004 		/* user */
#define PTE_PWT				0x008 		/* write-Through */
#define PTE_PCD				0x010 		/* cache-disabled */
#define PTE_A				0x020 		/* page accessed */
#define PTE_D				0x040 		/* page dirty */
#define PTE_PS				0x080 		/* Page Size */
#define PTE_MBZ				0x180 		/* Bits must be zero */


/* Get address/flags out of page table or directory */

#define PTE_ADDR(pte)			(pte & ~0xFFF)
#define PTE_FLAGS(pte)			(pte & 0xFFF)


/* Page alignment operations */

#define PAGE_ALIGN(pte)			(pte=((pte&0xfffff000)+PAGE_SIZE))
#define PAGE_ALIGNED(pte)		(!(pte & 0x00000fff))
#define FLAG_ALIGN			    0x1
#define FLAG_NOALIGN			0x0


DEFINE_SPINLOCK(mem_lock)

/* mmap.c*/

struct w_mmap{
	w_uint32 size;
	w_uint32 base_addr_low,base_addr_high;
	w_uint32 length_low,length_high;
	w_uint32 type;
};

w_ptr read_mmap(struct w_multiboot_info* mbt);
void print_mmap_entry(struct w_mmap*);
void map_kernel();
w_ptr kmalloc(w_uint32 size, w_int32 align);


/* paging.c */

void set_page_directory(w_pde*);
void page_fault_handler(struct w_regs);
void map_page(w_pde*,w_uint32,w_pte);
int is_mapped(w_pde*, w_uint32);
void unmap_page(w_pde*,w_uint32);
void init_paging();
void invalidate_page(w_uint32);


/* alloc.c */

void init_alloc(struct w_multiboot_info*);
w_pte alloc_page_frame(w_uint32);
void free_page_frame(w_uint32);


/* kheap.c */

#define INIT_HEAP_SIZE 0x4000

struct w_block{

	w_uint32 start;
	w_uint32 end;
	LINKED_LIST_MEMBER(block_node);
};

struct w_heap{

	w_uint32 start_addr;
	w_uint32 size;
	LINKED_LIST(block_list_head);
};

void init_kheap();
void* kalloc(w_uint32);
void kfree(w_uint32);
void brk(struct w_heap*);

#endif
