/*
 * alloc.c
 * Wolfrath/Kriewall, 2013
 *
 * The allocator.  Functions used for allocating pages
 */

#include "mem.h"

#define ADDR_INDEX(x)			( x / 0x20000 )
#define ADDR_OFFSET(x)			( x % 0x20 )
#define NUM_INDEX			32768

/* Access to this map must be synchronized!! */

uint page_map[ NUM_INDEX ];


extern uint kern_start;
extern uint kern_end;
extern uint next_page;

static void mark_frame_used(uint addr){

	//acquire(&mem_lock);
	
	uint index = ADDR_INDEX(addr);
	uint offset = ADDR_OFFSET( (addr/0x1000) );
	page_map[index] |= 0x1 << offset;

	//release(&mem_lock);
}

static void mark_frame_empty(uint addr){

	//acquire(&mem_lock);
	
	uint index = ADDR_INDEX(addr);
	uint offset = ADDR_OFFSET( (addr/0x1000) );
	page_map[index] &= ~(0x1 << offset);
	
	//release(&mem_lock);
}

static uint check_frame(uint addr){

	//acquire(&mem_lock);

	uint index = ADDR_INDEX(addr);
	uint offset = ADDR_OFFSET( (addr/0x1000) );
	uint val = page_map[index] & (0x1 << offset);

	//release(&mem_lock);

	return val;
}


/* Find an available page frame */

static uint find_frame(uint usr){
	
	//acquire(&mem_lock);

	uint i, frame, offset, max;
	int index;

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
		return NULL;

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

	//release(&mem_lock);
	
	return frame * 0x1000;
}


/* Initialize the page allocator */

void init_alloc(w_multiboot_info* mbt){

	uint i;
	for(i=0; i< NUM_INDEX; i++)
		page_map[i] = 0xFFFFFFFF;

	struct w_mmap* mmap = mbt->mmap_addr;

	uint count = 0;
	uint kern = 0;


	/* Map all available pages to our page_map */

	while(mmap < mbt->mmap_addr + mbt->mmap_length) {

		if(mmap->type == 1){
			uint addr = mmap->base_addr_low;
			if(!PAGE_ALIGNED(addr))
				PAGE_ALIGN(addr);
			
			for(i=addr; i<addr + mmap->length_low; i+=0x1000){
				if(i<(uint)&kern_start || i>=(uint)&kern_end){
					mark_frame_empty(i);
					count++;
				}
				else{
					kern++;
				}
			}
			
		}
		mmap = (struct w_mmap*)((uint)mmap+mmap->size+sizeof(uint));
	}

	/* Map Null frame used */

	mark_frame_used(0);

	printf("%d kernel pages and %d free pages!\n", kern, count);
}


/* Allocate an available page */

w_pte alloc_page_frame(uint flags){

	/* Allocate an aligned page. Use uint to avoid pointer arithmetic */

	w_pte addr = (w_pte) find_frame( flags & 0xFFFFFFFB );


	if(addr == NULL)
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

void free_page_frame(uint addr){
	mark_frame_empty(addr);
}
