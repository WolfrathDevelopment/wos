/*
 * task.c
 * Wolfrath/Kriewall, 2013
 *
 * Implements task functions
 */

#include "proc.h"

struct w_proc current_proc;
struct w_thread current_thread;

extern struct w_tss current_tss;
extern w_pde* kernel_page_directory;


// access to this int must be synchronized!
uint next_pid = 1;

void dummy_execution(){

    printf("New Process Exec");
    for(;;)
        ;
}

void context_switch(struct w_proc* next){

	//we also need to save the kernel stack pointer in tss

    char* nstack = kalloc(0x1000);
    nstack += 0xFFF;

    asm volatile("mov %0, %%esp" : : "r" (nstack));
    asm volatile("mov %0, %%ebp" : : "r" (nstack));
    asm volatile("jmp *%0" : : "r" (&dummy_execution));
}

void begin_multitasking(){

    /* Lets create a kernel clean-up thread */

    current_proc.sys = LINUX;
    current_proc.main = &current_thread;
    current_proc.pg_dir = kernel_page_directory;

    current_thread.parent = &current_proc;
    current_thread.pid = next_pid++;
    current_thread.flags = 0;
    current_thread.state = CREATED;

    //context_switch(&current_proc);
}

uint fork(){


}


