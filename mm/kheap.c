/*
 * kheap.c
 * Joel Wolfrath, 2014
 *
 * Provides for kernel dynamic memory allocation.
 */


#include <io/console.h>
#include <mm/mem.h>
#include <mm/paging.h>
#include <mm/ppa.h>

/* Initial kernel heap */

struct w_heap kern_heap;

extern PageDirectory init_pgdir;

static uint32_t next_alloc_address;


#define HEAP_OVERHEAD sizeof(struct w_block)


/* Set up the kernel heap */

void init_kheap(){

	/* Allocate first heap page at end of kernel binary */

    PageFrameIndex newPage = alloc_page();

	PageTblEntry phys_page;
    phys_page.page_frame = newPage;
    phys_page.present = 1;
    phys_page.writable = 1;

	uint32_t end = (uint32_t) ALIGN(&kern_end, PAGE_SIZE);

	uint32_t heap_start = KVIRT( end );

	page_map_virtual(&init_pgdir, heap_start, phys_page);
	zero((void*)heap_start, PAGE_SIZE);
	uint32_t size = 0x1000;


	/* Allocate 3 more pages for inital heap */

	while( size < INIT_HEAP_SIZE ) {
    
        newPage = alloc_page();
        phys_page.page_frame = newPage;

		page_map_virtual(&init_pgdir, heap_start + size, phys_page);
		zero(((void*)(heap_start + size)), PAGE_SIZE);
		size += 0x1000;
	}

	next_alloc_address = heap_start + size;


	/* Set up initial linked list of available blocks */

	struct w_block* kfirst_block;
	kfirst_block = (struct w_block*)heap_start;

	kfirst_block->start = heap_start + HEAP_OVERHEAD;
	kfirst_block->end = heap_start + INIT_HEAP_SIZE;

	kfirst_block->block_node.next = NULL;

	kern_heap.start_addr = heap_start;
	kern_heap.size = INIT_HEAP_SIZE;

	INIT_LIST_HEAD(kern_heap.block_list_head);

	list_add(&kern_heap.block_list_head, &kfirst_block->block_node);

	printf("kern heap: 0x%p\n", kern_heap.start_addr);
}

/* Debug contents of a heap */

static void heap_dump(struct w_heap* heap){

	printf("HEAP DUMP:\n");

	struct w_listnode* it = FIRST_NODE(heap->block_list_head);

	uint32_t count = 0;

	while(it){

		struct w_block* data = LIST_ENTRY(it, struct w_block, block_node);
		printf("    %p. node: 0x%p, start: 0x%p, end:0x%p\n",count++, it, data->start, data->end);
		it = NEXT_NODE(it);
	}
}


/* Dynamic allocation function for kernel */

void* kalloc(uint32_t size){

	struct w_heap* heap = &kern_heap;
	struct w_listnode* it = FIRST_NODE(heap->block_list_head);
	struct w_block* blk;

	/* We need a new list node which holds a block pointer.
	 * This will allow us to add to free list once freed
	 */

	uint32_t eff_size = size + HEAP_OVERHEAD;


	while( it != NULL){

		blk = LIST_ENTRY(it, struct w_block, block_node);

		if((blk->end - blk->start) > eff_size){

			/* Found a big enough block! */

			break;
		}

		it = it->next;
	}

	if(it == NULL){

		/* Not enough contiguous memory available */

        return NULL;
	}

	/* Remove this node from the free list */

	struct w_listnode* alloc_node = list_remove( &heap->block_list_head, it);

	uint32_t start = blk->start;
	blk->start += eff_size;

	/* First initizlize the block */

	struct w_block* nblock = (struct w_block*)(start + size);
	nblock->start = start;
	nblock->end = start + size;

	/* swap the two nodes to keep descriptors in order */

	//struct w_listnode temp = nblock->block_node;
	//nblock->block_node = *alloc_node;
	//*alloc_node = temp;

	uint32_t t1,t2;
	t1 = nblock->start;
	t2 = nblock->end;

	nblock->start = blk->start;
	nblock->end = blk->end;
	blk->start = t1;
	blk->end = t2;

    /* Is there still enough room for another allocation? */

	if((blk->end - blk->start) > 0)
		list_add( &heap->block_list_head, &nblock->block_node);

	//list_remove(&heap->block_list_head, &nblock->block_node);
	//list_add(&heap->block_list_head, alloc_node);

	heap_dump(heap);

	return (void*)start;
}


/* Combine adjacent free blocks into one larger block */

static int merge(struct w_heap* heap){

	uint32_t brk = 0;
	struct w_listnode* it1 = FIRST_NODE(heap->block_list_head);
	struct w_listnode* it2 = it1;

	while(it1->next){

		struct w_block* b1 = LIST_ENTRY(it1, struct w_block, block_node);
		it2 = it1->next;

		while(it2){

			struct w_block* b2 = LIST_ENTRY(it2, struct w_block, block_node);

			/* List is unsorted... figure out block order */

			if(b2->start > b1->start){

				/* block 1 then block 2 */

				if(b1->end + HEAP_OVERHEAD + 1 >= b2->start){
					printf("HEAP MERGE\n");
					b1->end = b2->end;
					list_remove( &heap->block_list_head, it2);
					zero(b2, sizeof(struct w_block));
					brk = 1;
					break;
				}
			}
			else{

				/* block 2 then block 1 */

				if(b2->end + HEAP_OVERHEAD + 1 >= b1->start){
					printf("HEAP MERGE\n");
					b2->end = b1->end;
					list_remove( &heap->block_list_head, it1);
					zero(b1, sizeof(struct w_block));
					brk = 1;
					break;
				}
			}

			it2 = it2->next;
		}

		if(brk){
			heap_dump(heap);
			return 1;
		}

		it1 = NEXT_NODE(it1);
	}

	return 0;
}

/* Free dynamic allocation beginning at va */

void kfree(uint32_t va){

	struct w_heap* heap = &kern_heap;
	struct w_block* blk = (struct w_block*)(va - HEAP_OVERHEAD);

	uint32_t size = blk->end - blk->start;
	zero((void*)blk->start, size);

	list_add( &heap->block_list_head, &blk->block_node);

	while(merge(heap))
		;
}

void brk(struct w_heap* heap){

}
