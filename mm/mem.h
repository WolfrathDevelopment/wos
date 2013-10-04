/*
* mem.h
* Wolfrath/Kriewall, 2013
*
* Defs for the x86 MMU
* Management for physical memory
*/

#ifndef MEM_H
#define MEM_H

#include "../boot/boot.h"
#include "../boot/multiboot.h"
#include "../core/core.h"

extern uint kern_start;
extern uint kern_end;

/* Ready to get your hands dirty?? */

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
	------------------------------------------------------------
	
	------------------------------------------------------------
	|	HEAP (grows up)
	----------------------------------------------0xC0103000
	|	BSS SEGMENT *
	----------------------------------------------0xC0102000
	|	DATA SEGMENT *
	----------------------------------------------0xC0101000
	|	TEXT SEGMENT (ELF) *
	----------------------------------------------0xC0100000
	|	DMA area
	----------------------------------------------0xC0000000

*/	

/* First kernel reserved address */
#define KMAP_START 0xC0000000

#define KPHYS(x) (x - KMAP_START)
#define KVIRT(x) (x + KMAP_START)

#define NUM_PDE 1024
#define NUM_PTE 1024
#define PAGE_SIZE 0x1000

/* Page directory and table index */
#define PD_INDEX(va) (va >> 22)
#define PT_INDEX(va) ((va >> 12) & 0x3FF)

/* Page table/directory flags */
#define PTE_P 0x001		// present
#define PTE_W 0x002 		// writeable
#define PTE_U 0x004 		// user
#define PTE_PWT 0x008 		// write-Through
#define PTE_PCD 0x010 		// cache-disabled
#define PTE_A 0x020 		// page accessed
#define PTE_D 0x040 		//page dirty
#define PTE_PS 0x080 		// Page Size
#define PTE_MBZ 0x180 		// Bits must be zero

/* Get address/flags out of page table or directory */
#define PTE_ADDR(pte) ((uint)(pte) & ~0xFFF)
#define PTE_FLAGS(pte) ((uint)(pte) & 0xFFF)

/* Page alignment operations */
#define PAGE_ALIGN(pte) (pte=((((w_pte)pte)&0xfffff000)+PAGE_SIZE))
#define PAGE_ALIGNED(pte) ((pte & 0xfffff000) || pte == 0)
#define FLAG_ALIGN 1
#define FLAG_NOALIGN 0

DEFINE_SPINLOCK(mem_lock)

/* mmap.c*/

typedef struct{
	unsigned int size;
	unsigned int base_addr_low,base_addr_high;
	unsigned int length_low,length_high;
	unsigned int type;
} w_mmap;

w_mmap * read_mmap(w_multiboot_info* mbt);
void print_mmap_entry(w_mmap*);
uint* kmalloc(uint size, int align);

/* paging.c */

void set_page_directory(w_pde*);
void page_fault_handler(w_regs);
void init_paging();
void invalidate_page(uint);

/* alloc.c */

w_pte alloc_page_frame(uint);

#endif
