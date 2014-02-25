/*
 * heap.c
 * Wolfrath/Kriewall, 2013
 *
 * Implements heap functions
 */

#include "mem.h"

struct w_heap kern_heap;
static struct w_list kern_list;

extern w_pde init_pgdir[];
extern w_pde* kernel_page_directory;

/* Set up the kernel heap */

void init_kheap(){

	w_pte phys_page = alloc_page_frame( PTE_P | PTE_W );
	
	uint end = ((uint)&kern_end);
	
	if(!PAGE_ALIGNED(end))
		PAGE_ALIGN(end);

	uint heap_start = KVIRT( end );

	printf("heap start: 0x%p\n", heap_start);

	map_page( kernel_page_directory, heap_start, phys_page );
	zero_mem(heap_start, PAGE_SIZE);

	uint size = 0x1000;

	while( size < INIT_HEAP_SIZE ){
		map_page( kernel_page_directory, heap_start + size, alloc_page_frame( PTE_P | PTE_W ));
		zero_mem(heap_start + size, PAGE_SIZE);
		size += 0x1000;
	}

	struct w_block* kfirst_block;
	struct w_listnode* kfirst_node;
	uint overhead = sizeof(struct w_listnode) + sizeof(struct w_block);

	kfirst_node = (struct w_listnode*)heap_start;
	kfirst_block = (struct w_block*)(heap_start + sizeof(struct w_listnode));

	kfirst_block->start = heap_start + overhead;
	kfirst_block->end = heap_start + INIT_HEAP_SIZE;

	kfirst_node->data = ((void*) kfirst_block);
	kfirst_node->next = NULL;

	kern_list.size = 0;
	kern_list.head = NULL;

	kern_heap.start_addr = heap_start;
	kern_heap.size = INIT_HEAP_SIZE;
	kern_heap.block_list = &kern_list;

	list_add( kern_heap.block_list, kfirst_node);
}

static void heap_dump(struct w_heap* heap){

	printf("HEAP DUMP:\n");
	
	struct w_listnode* it = heap->block_list->head;
	uint count = 1;

	while(it){
		struct w_block* data = (struct w_block*)it->data;
		printf("    %p. node: 0x%p, start: 0x%p, end:0x%p\n",count++, it, data->start, data->end);
		it = it->next;
	}
}

void* alloc(struct w_heap* heap, uint size){

	struct w_listnode* it = heap->block_list->head;
	struct w_block* blk;

	/* We need a new list node which holds a block pointer.
	 * This will allow us to add to free list once freed
	 */
	uint overhead = sizeof(struct w_listnode) + sizeof(struct w_block);
	uint eff_size = size + overhead;

	while( it != NULL){

		blk = ((struct w_block*)it->data);

		if((blk->end - blk->start) > eff_size){

			/* Found a big enough block! */
			break;
		}
	}

	if(it == NULL){
		/* Not enough contiguous memory available */
		return NULL;
	}

	/* Remove this node from the free list */
	struct w_listnode* last = list_remove( heap->block_list, it);

	uint start = blk->start;
	blk->start += eff_size;

	/* Is there still enough room for another allocation? */
	if((blk->end - blk->start) > 0)
		list_add( heap->block_list, last);
	
	/* First comes listnode struct then block */
	struct w_listnode* nnode = ((struct w_listnode*)start + size);
	struct w_block* nblock = (struct w_block*)(start+size+sizeof(struct w_listnode));
	nblock->start = start;
	nblock->end = start + size;

	/* swap the two nodes */
	nnode->data = last->data;
	last->data = (void*)nblock;
	list_remove(heap->block_list, last);
	list_add(heap->block_list, nnode);

	heap_dump(heap);

	return start;
}

static void merge(struct w_heap* heap){

	uint overhead = sizeof(struct w_listnode) + sizeof(struct w_block);
	uint inc = 1;
	struct w_listnode* it1 = heap->block_list->head;
	struct w_listnode* it2 = it1;

	while(it1->next){

		struct w_block* b1 = ((struct w_block*)it1->data);
		it2 = it1->next;

		printf("block1 start: 0x%p, block1 end: 0x%p\n", b1->start,b1->end);

		while(it2){

			struct w_block* b2 = ((struct w_block*)it2->data);

			printf("block2 start: 0x%p, block2 end: 0x%p\n", b2->start,b2->end);

			/* List is unsorted... figure out block order */

			if(b2->start > b1->start){

				/* block 1 then block 2 */
				if(b1->end + overhead + 1 >= b2->start){
					printf("HEAP MERGE\n");
					b1->end = b2->end;
					list_remove( heap->block_list, it2);
					zero_mem((uint)b2, sizeof(struct w_block));
					zero_mem((uint)it2, sizeof(struct w_listnode));
					break;
				}
			}
			else{

				/* block 2 then block 1 */
				if(b2->end + overhead + 1 >= b1->start){
					printf("HEAP MERGE\n");
					inc = 0;
					struct w_listnode* del = it1;
					it1 = it1->next;
					b2->end = b1->end;
					list_remove( heap->block_list, del);
					zero_mem((uint)b1, sizeof(struct w_block));
					zero_mem((uint)del, sizeof(struct w_listnode));
					break;
				}
			}

			it2 = it2->next;
		}

		if(inc)
			it1 = it1->next;
		else
			inc = 1;
	}

	struct w_block* nblk = (struct w_block*)heap->block_list->head->data;
	printf("heap start:0x%p, end:0x%p\n", nblk->start, nblk->end);
}

void free(struct w_heap* heap, uint va){

	uint overhead = sizeof(struct w_listnode) + sizeof(struct w_block);
	struct w_listnode* node = (struct w_listnode*)(va - overhead);

	struct w_block* blk = (struct w_block*)node->data;
	uint size = blk->end - blk->start;
	zero_mem(blk->start, size);

	list_add( heap->block_list, node);
	heap_dump(heap);

	merge(heap);
}
