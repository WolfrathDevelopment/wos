/*
 * acpi.c
 * Joel Wolfrath 2014
 *
 * Implementation of ACPI functions
 */

#include "acpi.h"
#include "../mm/mem.h"
#include "../core/core.h"

extern w_uint32 next_alloc_address;
extern w_pde* kernel_page_directory;

w_uint8 chkptr(w_int8* start, w_uint32 size){

	w_uint8* ptr = (w_uint8*)start;
	int i;
	w_uint8 chksm=0;

	for(i=0;i<size;i++)
		chksm += *ptr++;

	return chksm;
}

void init_acpi(){

	/* We need to find the RSDP... */

	struct w_rsdp* rsdp_ptr = NULL;
	struct w_rsdt* rsdt_ptr = NULL;

	w_int8* start = (w_int8*)0x000E0000;
	w_int8* end = (w_int8*)0x000FFFFF;
	start = KVIRT(start);
	end = KVIRT(end);

	printf("start: %p end: %p\n", start, end);

	while(start < end){

		/* Multiple APIC Description TBL */

		if(!memcmp(RSDP_SIG,start,8)){

			if(!chkptr(start,sizeof(struct w_rsdp))){

				rsdp_ptr = (struct w_rsdp*)start;
       			printf("sig: %s\n", rsdp_ptr->signature);
        		printf("checksum: %p\n", (w_uint32)rsdp_ptr->checksum);
       			printf("oemid: %s\n", rsdp_ptr->oemid);
        		printf("rev: %p\n", (w_uint32)rsdp_ptr->rev);
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
	w_uint32 rsdt = (w_uint32)rsdt;

	if(!is_mapped(kernel_page_directory, rsdt)){

		printf("Not mapped\n");

		if(!(kernel_page_directory[ PD_INDEX(rsdt) ] & PTE_P)){

			printf("Need pgtbl\n");

			/* Oh god, we need a page table too */

			w_pte page = alloc_page_frame(PTE_W | PTE_P);
			map_page(kernel_page_directory,next_alloc_address,page);
			kernel_page_directory[PD_INDEX(rsdt)] = page;
			zero_mem(next_alloc_address, 0x1000);
			next_alloc_address+=0x1000;
		}
		
		printf("Need page\n");

		/* Hell, let's just assume its page aligned */

		map_page(kernel_page_directory,KVIRT(rsdt),rsdt | PTE_W | PTE_P);
	}

	printf("sig: %s\n", rsdt_ptr->signature);
	printf("checksum: %p\n", (w_uint32)rsdt_ptr->checksum);
	printf("oemid: %s\n", rsdt_ptr->oemid);
	printf("rev: %p\n", (w_uint32)rsdt_ptr->rev);
	printf("length: 0x%p\n", rsdt_ptr->length);

}
