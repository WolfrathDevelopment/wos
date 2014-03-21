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

extern struct w_tss current_tss;
extern w_pde* kernel_page_directory;
extern w_uint32 next_alloc_address;
extern w_uint32* core_stack;

//extern void perform_context_switch(uint,uint);


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

    //set_page_directory(next->pg_dir);

    //printf("esp: 0x%p\n", next->esp);

    //current_proc = next;

    //set tss

    //perform_context_switch(next->esp,0);

    asm volatile("mov %0, %%esp" : : "r" (next->regs->esp));

    asm volatile("popl %ebx");
    asm volatile("mov %bx,%ds");
    asm volatile("mov %bx,%es");
    asm volatile("mov %bx,%fs");
    asm volatile("mov %bx,%gs");
    asm volatile("popa");
    asm volatile("addl $0x8, %esp");
    asm volatile("sti");
    asm volatile("iret");
}

void begin_multitasking(){

    /* Lets create a kernel clean-up thread */

    proc1.sys = LINUX;
    proc1.pg_dir = kernel_page_directory;
    proc1.pid = next_pid++;
    proc1.flags = 0;
    proc1.state = CREATED;
    proc1.next = &proc1;
    current_proc = &proc1;

    //proc2.sys = LINUX;
    //proc2.pg_dir = kernel_page_directory;
    //proc2.pid = next_pid++;
    //proc2.flags = 0;
    //proc2.state = CREATED;

    //char* nstack = kalloc(0x1000);
    //uint* nesp;
    //uint* nebp;

    //printf("\nStack alloc: 0x%p\n");

    //nstack += 0xFFF;

    //printf("\nNEW STACK START: 0x%p\n",nstack);

    // we need to build current stack for an iret into new process.

    //asm volatile("mov %0, %%esp" : : "r" (nstack));
    //asm volatile("mov %0, %%ebp" : : "r" (nstack));
    //asm volatile("jmp *%0" : : "r" (&dummy_execution));

    //current_proc = &proc1;
    //perform_context_switch(nstack,(uint)&dummy_execution);

    //nebp = (uint*)nstack;
    //nesp = nebp;

    //*nesp++ = 0x10;
    //*nesp++ = read_eflags();
    //*nesp++ = nebp;
    //*nesp++ = 0x8;
    //*nesp = &dummy_execution;

    //uint esp;

    //asm volatile("movl %%esp, %0" : "=r"(esp));
    //asm volatile("pushl $0x10");
    //asm volatile("pushfl");
    //asm volatile("pushl %0" : : "r" (nstack));
    //asm volatile("pushl $0x8");
    //asm volatile("pushl %0" : : "r" (&dummy_execution));

    //asm volatile("sti");
    //asm volatile("iret");

    //perform_context_switch(&current_proc);
    //while(1)
       // printf("PROC 1");

    w_uint32 eip = get_eip();

    printf("EIP = %p\n", eip);
    printf("&EIP = %p\n", &eip);
    while(1);

    fork(eip);
}

int fork(w_uint32 next_eip){

    /* Alright, lets allocate a new process */
    struct w_proc* pcopy = kalloc(sizeof(struct w_proc));

    pcopy->next = current_proc;
    current_proc->next = pcopy;

    pcopy->pid = next_pid++;
    //pcopy->thread_state = CREATED;
    //pcopy->thread_system = current_proc->thread_system;
    pcopy->flags = current_proc->flags;
    pcopy->regs = NULL;

    if( ! PAGE_ALIGNED(next_alloc_address))
        PAGE_ALIGN(next_alloc_address);

    /* Alloc new page directory */

    w_pde* new_pgdir;

    w_pte page = alloc_page_frame(PTE_W | PTE_P);
    map_page(kernel_page_directory, next_alloc_address, page);

    new_pgdir = (w_pde*)next_alloc_address;
    next_alloc_address += 0x1000;

    /* Copy Page Directory */

    int i;
    for(i = 0; i < 0x400; i++){
        new_pgdir[i] = kernel_page_directory[i];
    }

    pcopy->pg_dir = new_pgdir;

    /* Alloc stack */
    w_uint32* new_stack;

    page = alloc_page_frame(PTE_W | PTE_P);
    map_page(kernel_page_directory, next_alloc_address, page);

    new_stack = (uint*)(next_alloc_address);
    next_alloc_address += 0x1000;

    /* Copy Stack */

    w_uint32 nesp = (w_uint32)copy_stack(new_stack, core_stack);
    w_uint32 oesp = get_esp();

    set_esp(nesp);

    //new stack
    asm volatile("movl %0, %%esp" : : "r" (nesp));
    asm volatile("push %ss");
    asm volatile("pushl %esp");
    asm volatile("pushfl");
    asm volatile("push %cs");
    asm volatile("pushl %0" : : "r" (next_eip));
    asm volatile("pushl $0");
    asm volatile("pushl $0");
    asm volatile("pusha");
    asm volatile("push %ds");

    //go back to current stack
    //asm volatile("movl %0, %%esp" : : "r" (esp));

    //set 0 return value
    asm volatile("movl %0, %%eax" : : "r" (0));

    return pcopy->pid;
}


