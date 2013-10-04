/*
 * mmap.c
 * Wolfrath/Kriewall, 2013
 *
 * Implementation of mmap functions
 */

#include "mem.h"

#define INTS_TO_LONG(x,y) ((((ulong)x) << 32 ) | y);

static w_multiboot_info* mboot;

w_mmap * read_mmap(w_multiboot_info* mbt){

	mboot = mbt;
	w_mmap* mmap = mbt->mmap_addr;

	while(mmap < mbt->mmap_addr + mbt->mmap_length) {

		print_mmap_entry(mmap);
		mmap = (w_mmap*) ((uint)mmap + mmap->size + sizeof(uint));
	}

	return mbt->mmap_addr;
}

/* A allocation function whose memory will never be freed! */
uint* kmalloc(uint size, int align){

	//acquire(&mem_lock);
	
	uint addr = NULL;
	w_mmap* mmap = mboot->mmap_addr;
	ulong length = (ulong)size;

	/* Iterate over the memory map */
	while(mmap < mboot->mmap_addr + mboot->mmap_length) {

		/* How long is this section of memory? */
		ulong mem_length = INTS_TO_LONG(mmap->length_high, mmap->length_low);

		/* Is it long enough for this allocation? */		
		if(length <= mem_length && mmap->type == 0x1){
			
			/* We found enough memory! */
			addr = mmap->base_addr_low;

			/* Page alignment */
			if(align && (!PAGE_ALIGNED(addr))){

				PAGE_ALIGN(addr);
			
				if(addr + length > mem_length)
					continue;
				else
					mmap->base_addr_low = addr;

			}
			
			/* Lets remove this memory from the memory map */
			ulong new_addr =  addr + length;
			mmap->base_addr_high = 0;
			mmap->base_addr_low = (uint)new_addr;
			mem_length -= length;
			mmap->length_high = (uint)(mem_length >> 32);
			mmap->length_low = (uint)(mem_length & 0xFFFFFFFF);
			
			break;
			
		}
		mmap = (w_mmap*) ((uint)mmap + mmap->size + sizeof(uint));
	}

	//release(&mem_lock);

	return (uint*) addr;
}

void print_mmap_entry(w_mmap* entry){

	printf("0x%p%p ", entry->base_addr_high, entry->base_addr_low);
	printf("0x%p%p ", entry->length_high, entry->length_low);
	printf("0x%p \n", entry->type);
}
