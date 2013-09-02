/*
 * mmap.c
 * Wolfrath/Kriewall, 2013
 *
 * Implementation of mmap functions
 */

#include "mem.h"

MMAP_e * read_mmap(Multiboot_info* mbt){

	MMAP_e* mmap = mbt->mmap_addr;

	while(mmap < mbt->mmap_addr + mbt->mmap_length) {

		print_mmap_entry(mmap);
		mmap = (MMAP_e*) ((uint)mmap + mmap->size + sizeof(uint));
	}

	// ****TIM**** This should return a linked list of MMAP_e
	return 0;
}

void print_mmap_entry(MMAP_e* entry){

	put_string("0x");
	put_hex(entry->base_addr_high);
	put_hex(entry->base_addr_low);

	put_string(" 0x");
	put_hex(entry->length_high);
	put_hex(entry->length_low);

	put_string(" 0x");
	put_hex(entry->type);

	put_char('\n');
}