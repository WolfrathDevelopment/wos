/*
 * acpi.c
 * Joel Wolfrath 2014
 *
 * Implementation of ACPI functions
 */

#include <boot/acpi.h>
#include <mm/mem.h>
#include <mm/paging.h>
#include <lib/core.h>

extern uint32 next_alloc_address;
extern PageDirectoryEntry* kernel_page_directory;

uint8 validateChecksum(int8* start, uint32 size){

	uint8* ptr = (uint8*)start;
	uint8 chksm=0;
	int i;

	for(i=0; i<size; i++)
		chksm += *ptr++;

	return chksm;
}

void init_acpi(){

	/* We need to find the RSDP... */

	RootSystemDescPointer*	rsdp_ptr = NULL;
	RootSystemDescTable*	rsdt_ptr = NULL;

	int8* start = (int8*) 0x000E0000;
	int8* end = (int8*) 0x000FFFFF;

	start = KVIRT(start);
	end = KVIRT(end);

	printf("start: %p end: %p\n", start, end);

	while(start < end){

		/* Multiple APIC Description TBL */

		if(!memcmp(RSDP_SIG,start,8)){

			if(!validateChecksum(start, sizeof(RootSystemDescPointer))){

				rsdp_ptr = (RootSystemDescPointer*) start;
       			printf("sig: %s\n", rsdp_ptr->signature);
        		printf("checksum: %p\n", (uint32)rsdp_ptr->checksum);
       			printf("oemid: %s\n", rsdp_ptr->oemid);
        		printf("rev: %p\n", (uint32)rsdp_ptr->rev);
        		printf("rsdt_addr: 0x%p\n", rsdp_ptr->rsdt_addr);

			}
		}

		start++;
	}

	if(rsdp_ptr == NULL){
		printf("Failed to load RSDP\n");
		return;
	}

	rsdt_ptr = KVIRT(rsdp_ptr->rsdt_addr);
	uint32 rsdt = (uint32)rsdt;

	if(!is_mapped(kernel_page_directory, rsdt)){

		printf("Not mapped\n");

		if( ! isPtePresent(kernel_page_directory[ PD_INDEX(rsdt) ])){

			printf("Need pgtbl\n");

			/* Oh god, we need a page table too */

			PageTableEntry page = alloc_page_frame(PTE_Present | PTE_Writable);
			map_page(kernel_page_directory,next_alloc_address,page);
			kernel_page_directory[PD_INDEX(rsdt)] = page;
			zero(next_alloc_address, 0x1000);
			next_alloc_address+=0x1000;
		}

		printf("Need page\n");

		/* Hell, let's just assume its page aligned */

		map_page(kernel_page_directory,KVIRT(rsdt),rsdt | PTE_Writable | PTE_Present);
	}

	printf("sig: %s\n", rsdt_ptr->signature);
	printf("checksum: %p\n", (uint32)rsdt_ptr->checksum);
	printf("oemid: %s\n", rsdt_ptr->oemid);
	printf("rev: %p\n", (uint32)rsdt_ptr->rev);
	printf("length: 0x%p\n", rsdt_ptr->length);

}
