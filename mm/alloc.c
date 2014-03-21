/*
 * alloc.c
 * Joel Wolfrath, 2013
 *
 * The allocator.  Functions used for allocating pages
 */

#include "mem.h"

#define ADDR_INDEX(x)			( x / 0x20000 )
#define ADDR_OFFSET(x)			( x % 0x20 )
#define NUM_INDEX			32768

/* Access to this map must be synchronized!! */

w_uint32 page_map[ NUM_INDEX ];


extern w_uint32 kern_start;
extern w_uint32 kern_end;
extern w_uint32 next_page;

static void mark_frame_used(w_uint32 addr){

	//acquire(&mem_lock);

	w_uint32 index = ADDR_INDEX(addr);
	w_uint32 offset = ADDR_OFFSET( (addr/0x1000) );
	page_map[index] |= 0x1 << offset;

	//release(&mem_lock);
}

static void mark_frame_empty(w_uint32 addr){

	//acquire(&mem_lock);

	w_uint32 index = ADDR_INDEX(addr);
	w_uint32 offset = ADDR_OFFSET( (addr/0x1000) );
	page_map[index] &= ~(0x1 << offset);

	//release(&mem_lock);
}

static w_uint32 check_frame(w_uint32 addr){

	//acquire(&mem_lock);

	w_uint32 index = ADDR_INDEX(addr);
	w_uint32 offset = ADDR_OFFSET( (addr/0x1000) );
	w_uint32 val = page_map[index] & (0x1 << offset);

	//release(&mem_lock);

	return val;
}


/* Find an available page frame */

static w_uint32 find_frame(w_uint32 usr){

	//acquire(&mem_lock);

	w_uint32 i, frame, offset, max;
	w_int32 index;


    /* Should map more kernel tables here!! */

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

void init_alloc(struct w_multiboot_info* mbt){

	w_uint32 i;
	for(i=0; i< NUM_INDEX; i++)
		page_map[i] = 0xFFFFFFFF;

	struct w_mmap* mmap = mbt->mmap_addr;

	w_uint32 count = 0;
	w_uint32 kern = 0;


	/* Map all available pages to our page_map */

	while(mmap < mbt->mmap_addr + mbt->mmap_length) {

		if(mmap->type == 1){

			w_uint32 addr = mmap->base_addr_low;
			if(!PAGE_ALIGNED(addr))
				PAGE_ALIGN(addr);

			for(i=addr; i<addr + mmap->length_low; i+=0x1000){

				if(i<(w_uint32)&kern_start || i>=(w_uint32)&kern_end){
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

w_pte alloc_page_frame(w_uint32 flags){

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

void free_page_frame(w_uint32 addr){
	mark_frame_empty(addr);
}
