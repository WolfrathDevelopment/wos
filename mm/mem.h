/*
* mem.h
* Wolfrath/Kriewall, 2013
*
* Defs for the x86 MMU
*/

#ifndef MEM_H
#define MEM_H

#include "../boot/boot.h"
#include "../boot/multiboot.h"

// Page directory and page table constants.
#define NUM_PDE 1024
#define NUM_PTE 1024
#define PAGE_SIZE 4096

// page directory index
#define PD_INDEX(va) (((uint)(va) >> 22) & 0x3FF)

// page table index
#define PT_INDEX(va) (((uint)(va) >> 12) & 0x3FF)

// construct virtual address from indexes and offset
#define GET_VIRT(d, t, o) ((uint)((d) << 22 | (t) << 12 | (o)))

// Page table/directory entry flags.
#define PTE_P 0x001 // Present
#define PTE_W 0x002 // Writeable
#define PTE_U 0x004 // User
#define PTE_PWT 0x008 // Write-Through
#define PTE_PCD 0x010 // Cache-Disable
#define PTE_A 0x020 // Accessed
#define PTE_D 0x040 // Dirty
#define PTE_PS 0x080 // Page Size
#define PTE_MBZ 0x180 // Bits must be zero

// Address in page table or page directory entry
#define PTE_ADDR(pte) ((uint)(pte) & ~0xFFF)
#define PTE_FLAGS(pte) ((uint)(pte) & 0xFFF)

//Alignment operations for a page
#define PAGE_ALIGN(pte) (pte = (uint*)(((uint)pte & 0xfffff000) + PAGE_SIZE))
#define PAGE_ALIGNED(pte) pte & 0xfffff000
#define FLAG_ALIGN 1
#define FLAG_NOALIGN 0

/* heap.c */

uint* kmalloc(uint size, int align);

/* paging.c */

void set_page_directory(w_pde*);
w_pte* alloc_page(uint*,uint);
w_pde* allocate_page_directory(int);
void page_fault_handler(Registers);
void init_paging();

/* mmap.c*/
typedef struct{
	unsigned int size;
	unsigned int base_addr_low,base_addr_high;
	unsigned int length_low,length_high;
	unsigned int type;
} MMAP_e;
MMAP_e * read_mmap(Multiboot_info* mbt);
void print_mmap_entry(MMAP_e*);

#endif
