/*
 * alloc.c
 * Joel Wolfrath, 2013
 *
 * The allocator.  Functions used for allocating pages
 */

#include "mem.h"

#define ADDR_INDEX(x)			( x / 0x20000 )
#define ADDR_OFFSET(x)			( x % 0x20 )
#define NUM_INDEX				(32768)

/* Access to this map must be synchronized!! */

uint32 page_map[ NUM_INDEX ];


extern uint32 kern_start;
extern uint32 kern_end;
extern uint32 next_page;

static void mark_frame_used(uint32 addr){

	uint32 index = ADDR_INDEX(addr);
	uint32 offset = ADDR_OFFSET( (addr/0x1000) );
	page_map[index] |= 0x1 << offset;
}

static void mark_frame_empty(uint32 addr){

	uint32 index = ADDR_INDEX(addr);
	uint32 offset = ADDR_OFFSET( (addr/0x1000) );
	page_map[index] &= ~(0x1 << offset);
}

static uint32 check_frame(uint32 addr){

	uint32 index = ADDR_INDEX(addr);
	uint32 offset = ADDR_OFFSET( (addr/0x1000) );
	uint32 val = page_map[index] & (0x1 << offset);

	return val;
}


/* Find an available page frame */

static uint32 find_frame(uint32 usr){

	uint32 i, frame, offset, max;
	int32 index;

	if(usr)
		max = NUM_INDEX;
	else
		max = 0x20;

	index = -1;

	for(i=0; i < max; i++){

		if(page_map[i] != 0xFFFFFFFF){

			/* found one */

			index = i;
			break;
		}
	}

	if(index == -1)
		return 0;

	offset = 31;

	for( i = 0x80000000; i > 0; i >>= 1){

		if( i & page_map[index] ){
			offset--;
			continue;
		}
		else{
			break;
		}
	}

	frame = ( index * 0x20 ) + offset;

	return frame * 0x1000;
}


/* Initialize the page allocator */

void init_alloc(GrubMultibootInfo* mbt){

	uint32 i;
	for(i=0; i< NUM_INDEX; i++)
		page_map[i] = 0xFFFFFFFF;

	GrubMemoryMapEntry* mmap = (GrubMemoryMapEntry*) mbt->mmap_addr;

	uint32 count = 0;
	uint32 kern = 0;


	/* Map all available pages to our page_map */

	while((uint32)mmap < mbt->mmap_addr + mbt->mmap_length) {

		if(mmap->type == 1){

			uint32 addr = mmap->base_addr_low;
			addr = alignAddress(addr, PAGE_SIZE);

			for(i=addr; i<addr + mmap->length_low; i+=0x1000){

				if(i<(uint32)&kern_start || i>=(uint32)&kern_end){
					mark_frame_empty(i);
					count++;
				}
				else{
					kern++;
				}
			}

		}

		mmap = (GrubMemoryMapEntry*)((uint32)mmap+mmap->size+sizeof(uint32));
	}

	/* Map Null frame used */

	mark_frame_used(0);

	printf("%d kernel pages totaling %d bytes\n",kern,kern*PAGE_SIZE);
	printf("%d free pages totaling %d bytes\n", count, count*PAGE_SIZE);
}


/* Allocate an available page */

PageTableEntry alloc_page_frame(uint32 flags){

	/* Allocate an aligned page. Use uint to avoid pointer arithmetic */

	PageTableEntry addr = (PageTableEntry) find_frame( flags & 0xFFFFFFFB );


	if((void*)addr == NULL)
		return addr;

	mark_frame_used(addr);

	/*
	 * First 20 bits are address, last 12 are flags
	 * For sanity's sake, lets force the flags into 12 bits
	 */

	flags &= 0xFFF;


	/* Add the flags */

	return addr | flags;
}

void free_page_frame(uint32 addr){
	mark_frame_empty(addr);
}
