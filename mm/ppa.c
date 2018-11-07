#include <mm/ppa.h>
#include <lib/core.h>
#include <lib/bitmap.h>

#define PPA_INDEX(pa)	((pa / PAGE_SIZE) / 32)
#define PPA_OFFSET(pa)	((pa / PAGE_SIZE) % 32)

PhysicalPageAllocator PPAInstance;

void ppa_init(GrubMultibootInfo * info)
{
	ASSERT(info); // Who gave us NULL?
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
	}

	if( 1 /* verbose TODO */)
	{
		printf("%d kernel pages marked\n", kernel_pages);
		printf("%d free pages marked\n", free_pages);
	}
}

Page alloc_page()
{
	Page foundPage;
	foundPage.all_fields = 0;

    lock(&PPAInstance.lock);

	int32_t map_index = -1;
	uint32_t map_offset = 31;
	uint32_t i, frame;

	/* TODO we can do better than O(n) */

	for(i = 0; i < MAX_PPA_ENTRIES; i++)
	{
		if(PPAInstance.bitmap[i] != 0xFFFFFFFF)
		{
			// Found a page
			map_index = i;
			break;
		}
	}

	if(map_index != -1)
	{
		for(i = 0x80000000; i > 0; i >>= 1)
		{
			if(i & PPAInstance.bitmap[i])
			{
				map_offset--;
			}
			else
			{
				break;
			}
		}

		SET_BIT(PPAInstance.bitmap[ map_index ], map_offset);
		foundPage.frame = (map_index * 32) + map_offset;
	}

    unlock(&PPAInstance.lock);

	return foundPage;
}

void free_page(Page currentPage)
{
    lock(&PPAInstance.lock);

	uint32_t map_index = PPA_INDEX(currentPage.frame);
	uint32_t map_offset = PPA_OFFSET(currentPage.frame);

	ASSERT(map_index < MAX_PPA_ENTRIES);
	ASSERT(	! IS_BIT_SET(PPAInstance.bitmap[map_index], map_offset)); // Page never allocated?

	CLR_BIT(PPAInstance.bitmap[ map_index ], map_offset);

    unlock(&PPAInstance.lock);
}
