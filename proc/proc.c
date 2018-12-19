/*
 * task.c
 * Joel Wolfrath, 2013
 *
 * Implements task functions
 */

#include <mm/paging.h>
#include <mm/mem.h>
#include <mm/ppa.h>
#include <proc/proc.h>
#include <lib/core.h>
#include <lib/string.h>

struct w_proc* current_proc = NULL;
struct w_proc proc1;
OsIsrFrame init_regs;

extern struct w_tss current_tss;
extern PageDirectory init_pgdir;
//extern uint32 next_alloc_address;
extern uint32* init_stack;

Task glb_kinit_task;

Task* get_current_task()
{
    return &glb_kinit_task;
}

// access to this int must be synchronized!
uint32 next_pid = 1;

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
    //set_page_directory(next->pg_dir);
    current_proc = next;

    //set tss

    asm volatile("mov %0, %%esp" : : "r" (next->regs->esp));
	pop_context();
}

void begin_multitasking(){

    /* Lets create a kernel clean-up thread */

    proc1.pg_dir = &init_pgdir;
    proc1.pid = next_pid++;
    proc1.flags = 0;
    proc1.state = CREATED;
    proc1.next = &proc1;
	proc1.regs = &init_regs;
    current_proc = &proc1;
}

int fork(){
    return 0;
}


