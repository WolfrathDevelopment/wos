/*
 * heap.c
 * Wolfrath/Kriewall, 2013
 *
 * Implements heap functions
 */

#include "mem.h"

#define KHEAP_START 0xF00000

// Lets start with 4 mb
#define KHEAP_SIZE 0x400000

#define KHEAP_STOP (KHEAP_START + KHEAP_SIZE)

uint* next_addr = KHEAP_START;

/* A allocation function whose memory will never be freed! */
uint* kmalloc(uint size, int align){

	if(align && !PAGE_ALIGNED(next_addr))
		PAGE_ALIGN(next_addr);

	if((next_addr + size) > KHEAP_STOP)
		return NULL;
	 
	uint* phy_addr = next_addr;
	next_addr += size;
	return phy_addr;
}
