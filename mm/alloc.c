/*
 * alloc.c
 * Joel Wolfrath, 2013
 *
 * The allocator.  Functions used for allocating pages
 */

#include <mm/mem.h>
#include <io/console.h>

/* TODO: Use max mem constants and PAGE_SIZE below */
#define ADDR_INDEX(x)			( x / 0x20000 )
#define ADDR_OFFSET(x)			( x % 0x20 )
#define NUM_INDEX				(32768)

/* Access to this map must be synchronized!! */

uint32_t page_map[ NUM_INDEX ];


extern uint32_t kern_start;
extern uint32_t kern_end;
extern uint32_t next_page;

static void mark_frame_used(uint32_t addr){

	uint32_t index = ADDR_INDEX(addr);
	uint32_t offset = ADDR_OFFSET( (addr/0x1000) );
	page_map[index] |= 0x1 << offset;
}

static void mark_frame_empty(uint32_t addr){

	uint32_t index = ADDR_INDEX(addr);
	uint32_t offset = ADDR_OFFSET( (addr/0x1000) );
	page_map[index] &= ~(0x1 << offset);
}

static uint32_t check_frame(uint32_t addr){

	uint32_t index = ADDR_INDEX(addr);
	uint32_t offset = ADDR_OFFSET( (addr/0x1000) );
	uint32_t val = page_map[index] & (0x1 << offset);

	return val;
}


/* Find an available page frame */

static uint32_t find_frame(uint32_t usr){

	uint32_t i, frame, offset, max;
	int32_t index;

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

	uint32_t i = 0;
	for(; i< NUM_INDEX; i++)
		page_map[i] = 0xFFFFFFFF;

	GrubMemoryMapEntry* mmap = (GrubMemoryMapEntry*) mbt->mmap_addr;

	uint32_t count = 0;
	uint32_t kern = 0;


	/* Map all available pages to our page_map */

	while((uint32_t)mmap < mbt->mmap_addr + mbt->mmap_length) {

		if(mmap->type == 1){

			uint32_t addr = mmap->base_addr_low;
			addr = ALIGN(addr, PAGE_SIZE);

			for(i=addr; i<addr + mmap->length_low; i+=0x1000){

				if(i<(uint32_t)&kern_start || i>=(uint32_t)&kern_end){
					mark_frame_empty(i);
					count++;
				}
				else{
					kern++;
				}
			}

		}

		mmap = (GrubMemoryMapEntry*)((uint32_t)mmap+mmap->size+sizeof(uint32_t));
	}

	/* Map Null frame used */

	mark_frame_used(0);

	printf("%d kernel pages totaling %d bytes\n",kern,kern*PAGE_SIZE);
	printf("%d free pages totaling %d bytes\n", count, count*PAGE_SIZE);
}


/* Allocate an available page */

PageTableEntry alloc_page_frame(uint32_t flags){

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

void free_page_frame(uint32_t addr){
	mark_frame_empty(addr);
}
