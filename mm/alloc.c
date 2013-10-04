/*
 * alloc.c
 * Wolfrath/Kriewall, 2013
 *
 * The allocator.  Functions used for allocating pages
 */

#include "mem.h"


 /* Allocate an available page */
w_pte alloc_page_frame(uint flags){

	/* Allocate an aligned page. Use uint to avoid pointer arithmetic */
	w_pte addr = (w_pte) kmalloc(PAGE_SIZE, FLAG_ALIGN);

	/* Zero out the page. */
	zero_mem((uint*)addr, PAGE_SIZE);

	/*
	 * First 20 bits are address, last 12 are flags
	 * For sanity's sake, lets force the flags into 12 bits
	 */
	flags &= 0xFFF;
	
	/* Add the flags */
	return addr | flags;
}
