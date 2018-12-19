#include <mm/ppa.h>
#include <lib/core.h>
#include <lib/bitmap.h>

#define PPA_INDEX(pa)	((pa / PAGE_SIZE) / 32)
#define PPA_OFFSET(pa)	((pa / PAGE_SIZE) % 32)

PhysicalPageAllocator PPAInstance;

void ppa_init(GrubMultibootInfo * info)
{
	ASSERT(info); // We need a memory map
    SPINLOCK_INIT(PPAInstance.lock);

	// Mark all pages as available initially
	memset32(PPAInstance.bitmap, 0xFFFFFFFF, MAX_PPA_ENTRIES);

	GrubMemoryMapEntry * mmap = (GrubMemoryMapEntry*) info->mmap_addr;
	uint32_t kernel_pages = 0;
	uint32_t free_pages = 0;

	/* Parse the memory map and assign pages appropriately */
	while((uint32_t)mmap < info->mmap_addr + info->mmap_length)
	{
		if(mmap->type == GRUB_MMAP_AVAILABLE)
		{
			uint32_t base_addr = mmap->base_addr_low;
			base_addr = ALIGN(base_addr, PAGE_SIZE);

			// TODO Use pointer arithmetic
			uint32_t max_addr = base_addr + mmap->length_low;
			for( ; base_addr < max_addr; base_addr += PAGE_SIZE)
			{
				if(	base_addr <  (uint32)&kern_start ||
					base_addr >= (uint32)&kern_end)
				{
					// mark_frame_empty
					free_pages++;
				}
				else
				{
					kernel_pages++;
				}
			}  
		}

        mmap = (GrubMemoryMapEntry*) ((uint32_t)mmap + mmap->size + sizeof(uint32_t));
	}

    printf("Kernel executable space:   %d KB\n", (kernel_pages * 4));
    printf("Physical memory available:  %d MB\n", (free_pages * 4) / 1024);
}

PageFrameIndex alloc_page()
{
    PageFrameIndex found_page = PageFrameIndexInvalid;

    lock(&PPAInstance.lock);

    int32_t bitmap_index = -1;
    uint32_t bitmap_index_offset = 0;

    /* TODO we can do better than O(n) */
    /* TODO do we need to hold the lock while we search? */

    uint32_t i = 0;
    for(i = 0; i < MAX_PPA_ENTRIES; i++)
    {
        if(PPAInstance.bitmap[i])
        {
            // Found a page
            bitmap_index = i;
            break;
        }   
    }

    if(bitmap_index != -1)
    {
        bitmap_index_offset = 31;
        for(i = 0x80000000; i != 0; i >>= 1)
        {
            if(i & PPAInstance.bitmap[bitmap_index])
            {
                break;
            }
            bitmap_index_offset--;
        }

	    PPAInstance.bitmap[bitmap_index] &= ~(1 << bitmap_index_offset);
        found_page = (bitmap_index * 32) + bitmap_index_offset;
    }

    unlock(&PPAInstance.lock);

    return found_page;
}

void free_page(PageFrameIndex frame)
{
    lock(&PPAInstance.lock);

	uint32_t map_index = PPA_INDEX(frame);
	uint32_t map_offset = PPA_OFFSET(frame);

	ASSERT(map_index < MAX_PPA_ENTRIES);
	ASSERT(PPAInstance.bitmap[map_index] ^ (1 << map_offset)); // Page never allocated?

	PPAInstance.bitmap[ map_index ] |= (1 << map_offset);

    unlock(&PPAInstance.lock);
}
