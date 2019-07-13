/*
 * paging.c
 * Joel Wolfrath, 2013
 *
 * Implementation of paging functions
 */

#include <types.h>
#include <arch/cpu.h>
#include <boot/isr.h>
#include <mm/mem.h>
#include <io/console.h>
#include <mm/paging.h>
#include <proc/proc.h>
#include <lib/core.h>
#include <tools/debug.h>

/* Initial page table(s) */
FORCE_ALIGN(PAGE_SIZE) PageTable init_pgtbl;

/* Initial page directory */
FORCE_ALIGN(PAGE_SIZE) PageDirectory init_pgdir;

/* Initial stack */
FORCE_ALIGN(PAGE_SIZE) uint8_t init_stack[PAGE_SIZE];

void update_page_directory()
{
    Task* current_task = get_current_task();
    if(current_task)
    {
        uint32_t cr0 = _cr0();

        // Give cr3 the address of this page directory
        asm volatile("mov %0, %%cr3":: "r"(current_task->page_directory));

        cr0 |= CR0_PAGING_ENABLED;

        // Write new cr0 value back to the register
        asm volatile("mov %0, %%cr0":: "r"(cr0));
    }
    else
    {
        PANIC("What exactly is running right now?");
    }
}

static inline PageTable* get_page_table_for_virtual_address(PageDirectory* dir, uint32_t va)
{
    PageTable* table = NULL;
    PageDirEntry pde = dir->tables[PDE_INDEX(va)];

    if(pde.present)
    {
        table = (PageTable*) KVIRT(pde.table_frame);
    }

    return table;
}

int32_t page_is_mapped(PageDirectory* dir, uint32_t va)
{
    PageTable* table = get_page_table_for_virtual_address(dir, va);
    if(table)
    {
        PageTblEntry pte = table->pages[PTE_INDEX(va)];
        return pte.present;
    }

    return FALSE;
}

/* Map a page to the virtual address in the given page directory */
void page_map_virtual(PageDirectory* dir, uint32_t virt_address, PageTblEntry pte)
{
    PageTable* table = get_page_table_for_virtual_address(dir, virt_address);
    if(!table)
    {
        // TODO allocate a physical page and map the page table
        PANIC("Punting for now");
    }

    /* Write the physical page to the page table */
    table->pages[ PTE_INDEX(virt_address) ] = pte;
}


/* Unmap the page at the given virtual address */
void page_unmap(PageDirectory* dir, uint32_t virt_address)
{
    PageTable* table = get_page_table_for_virtual_address(dir, virt_address);
    if(table)
    {
        /* Remove the entry in this page table */
        PageTblEntry zero_pte;
        zero_pte.all_fields = 0;
        table->pages[ PTE_INDEX(virt_address) ] = zero_pte;
    }
}

void init_paging()
{
    Task* current_task = get_current_task();
    isr_register(OsIsrPageFault, page_fault_handler);

    /* First physical page is off limits! */
    page_unmap(current_task->page_directory, 0);
    invlpg(0);
}

void page_fault_handler(OsIsrFrame* regs)
{
    uint32_t faulting_address = _cr2();

    printf("Page fault at 0x%p! eip=%p\n", faulting_address, regs->eip);
    printf("Error Code: %x\n", regs->err_code);

    if(regs->err_code & PAGE_FAULT_ERR_EXIST)
    {
        PANIC("Tried to access page that doesn't exist");
    }
}
