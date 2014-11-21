/*
 * task.c
 * Joel Wolfrath, 2013
 *
 * Implements task functions
 */

#include "proc.h"
#include "../core/core.h"

struct w_proc* current_proc = NULL;
struct w_proc proc1;
struct w_regs init_regs;

extern struct w_tss current_tss;
extern w_pde* kernel_page_directory;
extern w_uint32 next_alloc_address;
extern w_uint32* core_stack;
extern w_uint32* init_stack;

// access to this int must be synchronized!
w_uint32 next_pid = 1;

void context_switch(struct w_proc* next){

    /*
     * In order to preempt threads and switch contexts, the following
     * things need to happen:
     *
     * 1. Interrupt from PIC stops current thread execution and
     *    switches to the kernel code in the current page directory.
     *
     * 2. Kernel saves the state of the current thread and calls
     *    into the scheduler.
     *
     * 3. Scheduler chooses the next thread to run, then loads its page
     *    directory. Now kernel is executing in the new threads virtual
     *    memory space.
     *
     * 4. Kernel restores the state of the thread that is about to run.
     *
     * 5. Kernel saves its current stack pointer in the tss and
     *    switches to user mode (if it is a user mode process).
     */

    next->state = RUNNING;
    set_page_directory(next->pg_dir);
    current_proc = next;

    //set tss

    asm volatile("mov %0, %%esp" : : "r" (next->regs->esp));
	pop_context();
}

void begin_multitasking(){

    /* Lets create a kernel clean-up thread */

    proc1.sys = LINUX;
    proc1.pg_dir = kernel_page_directory;
    proc1.pid = next_pid++;
    proc1.flags = 0;
    proc1.state = CREATED;
    proc1.next = &proc1;
	proc1.regs = &init_regs;
    current_proc = &proc1;
}

int fork(){

    /* Alright, lets allocate a new process */

    struct w_proc* pcopy = kalloc(sizeof(struct w_proc));

    pcopy->next = current_proc;
    current_proc->next = pcopy;
    pcopy->pid = next_pid++;
    pcopy->flags = current_proc->flags;
    pcopy->regs = NULL;

    if( ! PAGE_ALIGNED(next_alloc_address))
        PAGE_ALIGN(next_alloc_address);

    /* Alloc new page directory */

    w_pde* new_pgdir;
	w_pte pdpage, nspage, ptpage;

    pdpage = alloc_page_frame(PTE_W | PTE_P);
    map_page(kernel_page_directory, next_alloc_address, pdpage);

    new_pgdir = (w_pde*)next_alloc_address;
    next_alloc_address += 0x1000;
    pcopy->pg_dir = new_pgdir;

    /* Allocate a stack */

    w_uint32* new_stack;
    nspage = alloc_page_frame(PTE_W | PTE_P);

	/* TEMPORARY MAP */
    map_page(kernel_page_directory, next_alloc_address, nspage);

    new_stack = (w_uint32*)(next_alloc_address);
	next_alloc_address += 0x1000;

    /* Copy Stack */

	w_uint32 csaddr = (w_uint32)(&init_stack + 0x400);
	w_uint32 oesp = get_esp();

	/* We are going to need 1 additional page table... */

	w_pte* new_pgtbl;
	ptpage = alloc_page_frame(PTE_W | PTE_P);
	new_pgtbl = (w_pte*)next_alloc_address;

	/* TEMPORARY MAP */
	map_page(kernel_page_directory, next_alloc_address,ptpage);
	next_alloc_address += 0x1000;

	/* Page table for original stack */

	w_pte* old_pgtbl;
	w_pde opde = kernel_page_directory[ PD_INDEX(csaddr) ];

	old_pgtbl = (w_pte*) KVIRT( PTE_ADDR(opde) );

	/* Now let's copy page directory, stack, and page table */

	int i;
	for(i=0;i<0x400;i++){
		new_pgdir[i] = kernel_page_directory[i];
	}

	memcpy((w_int8*)new_pgtbl,(w_uint8*)old_pgtbl, PAGE_SIZE);
    copy_stack(new_stack, &init_stack + 0x400, oesp);
	
	/* Alright, NOW remap the stack */

	new_pgdir[ PD_INDEX(csaddr) ] = (w_pde)(ptpage | PTE_A);
	
	map_page(new_pgdir, csaddr, nspage);

	w_uint32 eip = (w_uint32)&&forkret;
	oesp = get_esp();

//	for(;;)
//		;

    copy_stack(new_stack, &init_stack + 0x400, oesp);
	
	/* UNMAP NEW STACK AND PAGE TABLE */

	//unmap_page(kernel_page_directory, (w_uint32) new_pgtbl);
	//unmap_page(kernel_page_directory, (w_uint32) new_stack);

	cli();

	set_page_directory(new_pgdir);

	/* SAVE OUR CURRENT STACK! */

	asm volatile("pushfl");
	asm volatile("push %cs");
	asm volatile("pushl %0" : "=r" (eip));
	asm volatile("pushl $0");
	asm volatile("pushl $0");
	asm volatile("pushal");
	asm volatile("push %ds");

	set_page_directory(kernel_page_directory);
	set_esp(oesp);

	sti();

	/* This is the parent */

	return pcopy->pid;

forkret:

	/* Only the child will get this far */

	return 0;
}


