/*
 * mmap.c
 * Joel Wolfrath, 2013
 *
 * Implementation of functions that handle the
 * GRUB memory map
 */

#include "mem.h"


/* Macro to combine two 32 bit numbers into 64 bits */

#define INTS_TO_LONG(x,y) ((((w_uint64)x) << 32 ) | y)


/* Macro to get next entry in mmap */

#define MMAP_NEXT(x) ((w_uint32)x + x->size + sizeof(w_uint32))

static struct w_multiboot_info* mboot;

w_ptr read_mmap(struct w_multiboot_info* mbt){

	w_ptr ret;
	mboot = mbt;
	struct w_mmap* mmap = (struct w_mmap*) mbt->mmap_addr;


	/* Iterate over mmap and print out entries */

	while((w_uint32)mmap < mbt->mmap_addr + mbt->mmap_length) {

		print_mmap_entry(mmap);

		if(mmap->type == 0x3)
			ret = (w_ptr)mmap->base_addr_low;

		mmap = (struct w_mmap*)((w_uint32)mmap + mmap->size + sizeof(w_uint32));
	}

	return ret;
}

void map_kernel(){

	struct w_mmap* mmap = (struct w_mmap*) mboot->mmap_addr;
	struct w_mmap* entry = NULL;

	while((w_uint32)mmap < mboot->mmap_addr + mboot->mmap_length) {

		printf("map: 0x%p and kern: 0x%p\n", mmap->base_addr_low, &kern_start);
		if(mmap->base_addr_low == ((w_uint32)&kern_start)){
			entry = mmap;
			break;
		}
		mmap = (struct w_mmap*) ((w_uint32)mmap + mmap->size + sizeof(w_uint32));
	}

	if(entry){

		w_uint32 length = ((w_uint32)&kern_end) - ((w_uint32)&kern_start);


		/* Lets remove this memory from the memory map */

		w_uint64 new_addr =  entry->base_addr_low + length;
		entry->base_addr_high = 0;
		entry->base_addr_low = (w_uint32)new_addr;
		entry->length_low-=length;
	}
	else{
		PANIC("Failed to map kernel")
	}
}


/* A allocation function whose memory will never be freed! */

w_ptr kmalloc(w_uint32 size, int align){

	//acquire(&mem_lock);

	w_uint32 addr = (w_uint32) NULL;
	struct w_mmap* mmap = (struct w_mmap*) mboot->mmap_addr;
	w_uint64 length = (w_uint64)size;

	/* Iterate over the memory map */

	while((w_uint32)mmap < mboot->mmap_addr + mboot->mmap_length) {

		/* How long is this section of memory? */

		w_uint64 mem_length = INTS_TO_LONG(mmap->length_high, mmap->length_low);

		/* Is it long enough for this allocation? */

		if(length <= mem_length && mmap->type == 0x1){

			/* We found enough memory! */

			addr = mmap->base_addr_low;

			if(align && (!PAGE_ALIGNED(addr))){

				PAGE_ALIGN(addr);

				if(addr + length > mem_length)
					continue;
				else
					mmap->base_addr_low = addr;

			}

			/* Lets remove this memory from the memory map */

			w_uint64 new_addr =  addr + length;
			mmap->base_addr_high = 0;
			mmap->base_addr_low = (w_uint32)new_addr;
			mem_length -= length;
			mmap->length_high = (w_uint32)(mem_length >> 32);
			mmap->length_low = (w_uint32)(mem_length & 0xFFFFFFFF);

			break;

		}

		mmap = (struct w_mmap*) ((w_uint32)mmap + mmap->size + sizeof(w_uint32));
	}

	//release(&mem_lock);

	return (w_uint32*) addr;
}

void print_mmap_entry(struct w_mmap* entry){

	printf("0x%p%p ", entry->base_addr_high, entry->base_addr_low);
	printf("0x%p%p ", entry->length_high, entry->length_low);
	printf("0x%p \n", entry->type);
}
