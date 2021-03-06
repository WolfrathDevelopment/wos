/*
 * mmap.c
 * Joel Wolfrath, 2013
 *
 * Implementation of functions that handle the
 * GRUB memory map
 */

#include <io/console.h>
#include <mm/mem.h>

/* Macro to combine two 32 bit numbers into 64 bits */

#define INTS_TO_LONG(x,y) ((((uint64_t)x) << 32 ) | y)

#define GRUB_MMAP_TYPE_AVAIL    (0x3)

/* Macro to get next entry in mmap */

#define MMAP_NEXT(x) ((uint32_t)x + x->size + sizeof(uint32_t))

static GrubMultibootInfo* mboot;

void* read_mmap(GrubMultibootInfo* mbt){

	void* ret;
	mboot = mbt;
	GrubMemoryMapEntry* mmap = (GrubMemoryMapEntry*) mbt->mmap_addr;


	/* Iterate over mmap and print out entries */

	while((uint32_t)mmap < mbt->mmap_addr + mbt->mmap_length) {

		print_mmap_entry(mmap);

		if(mmap->type == GRUB_MMAP_TYPE_AVAIL)
			ret = (void*)mmap->base_addr_low;

		mmap = (GrubMemoryMapEntry*)((uint32_t)mmap + mmap->size + sizeof(uint32_t));
	}

    uint32_t length = ((uint32_t)&kern_end) - ((uint32_t)&kern_start);
    printf("Kernel binary size: %dK\n", length / 1024);

	return ret;
}

void map_kernel(){

	GrubMemoryMapEntry* mmap = (GrubMemoryMapEntry*) mboot->mmap_addr;
	GrubMemoryMapEntry* entry = NULL;

	while((uint32_t)mmap < mboot->mmap_addr + mboot->mmap_length) {

        //printf("map: 0x%p and kern: 0x%p\n", mmap->base_addr_low, &kern_start);
		if(mmap->base_addr_low == ((uint32_t)&kern_start)){
			entry = mmap;
			break;
		}
		mmap = (GrubMemoryMapEntry*) ((uint32_t)mmap + mmap->size + sizeof(uint32_t));
	}

	if(entry){

		uint32_t length = ((uint32_t)&kern_end) - ((uint32_t)&kern_start);
        printf("Kernel binary size: %x\n", length);

		/* Lets remove this memory from the memory map */

		uint64_t new_addr =  entry->base_addr_low + length;
		entry->base_addr_high = 0;
		entry->base_addr_low = (uint32_t)new_addr;
		entry->length_low-=length;
	}
	else{
		PANIC("Failed to map kernel");
	}
}


/* A allocation function whose memory will never be freed! */

void* kmalloc(uint32_t size, int align){

	uint32_t addr = (uint32_t) NULL;
	GrubMemoryMapEntry* mmap = (GrubMemoryMapEntry*) mboot->mmap_addr;
	uint64_t length = (uint64_t)size;

	/* Iterate over the memory map */

	while((uint32_t)mmap < mboot->mmap_addr + mboot->mmap_length) {

		/* How long is this section of memory? */

		uint64_t mem_length = INTS_TO_LONG(mmap->length_high, mmap->length_low);

		/* Is it long enough for this allocation? */

		if(length <= mem_length && mmap->type == 0x1){

			/* We found enough memory! */

			addr = mmap->base_addr_low;

			if(align){

				addr = ALIGN(addr, PAGE_SIZE);

				if(addr + length > mem_length)
					continue;
				else
					mmap->base_addr_low = addr;

			}

			/* Lets remove this memory from the memory map */

			uint64_t new_addr =  addr + length;
			mmap->base_addr_high = 0;
			mmap->base_addr_low = (uint32_t)new_addr;
			mem_length -= length;
			mmap->length_high = (uint32_t)(mem_length >> 32);
			mmap->length_low = (uint32_t)(mem_length & 0xFFFFFFFF);

			break;

		}

		mmap = (GrubMemoryMapEntry*) ((uint32_t)mmap + mmap->size + sizeof(uint32_t));
	}

	return (uint32_t*) addr;
}

void print_mmap_entry(GrubMemoryMapEntry* entry){

	//printf("0x%p%p ", entry->base_addr_high, entry->base_addr_low);
	//printf("0x%p%p ", entry->length_high, entry->length_low);
	//printf("0x%p \n", entry->type);
}
