/*
 * proc.h
 * Joel Wolfrath, 2013
 *
 */

#ifndef PROC_H
#define PROC_H

#include <mm/mem.h>
#include <boot/boot.h>
#include <types.h>
#include <lib/core.h>


/* What is the state of the given thread? */

enum thread_state {CREATED, RUNABLE, RUNNING, WAITING, SLEEPING, STOPPED};


/* Which system are we running on? */

enum thread_system { LINUX, OSX, WIN };

/*
struct context {
    uint32 edi;
    uint32 esi;
    uint32 ebx;
    uint32 ebp;
    uint32 eip;
};
*/

/* Defines a process */

struct w_proc{

	uint32 pid;
	enum thread_state state;
	enum thread_system sys;
	struct w_proc* next;
	PageDirectoryEntry* pg_dir;
	uint32 flags;
	Registers* regs;
}__attribute__((packed));


/* pic.c */

typedef void (*w_timer_callback)();

void init_pic();
void reset_pic(uint32);
void register_timer(w_timer_callback, uint32);


/* proc.c */

void begin_multitasking();
void context_switch(struct w_proc*);
int fork();
int exec();


/* thread.c */

//struct w_thread* create_thread(struct w_proc*, uint);

/* schedule.c */

void schedule();

/* stack.c */

void* get_eip();
void* copy_stack(uint32*, uint32*,uint32);

/*
 * If we want the current esp, we better not have
 * stack overhead from a function call...
 */

__attribute__((always_inline)) inline static uint32 get_esp(){

    uint32 esp;
    asm volatile("movl %%esp, %0" : "=r"(esp));
    return esp;
}

__attribute__((always_inline)) inline static void set_esp(uint32 esp){
    asm volatile("movl %0, %%esp" : : "r" (esp));
}

__attribute__((always_inline)) inline static void set_ss(uint16 ss){
    asm volatile("mov %0, %%ss" : : "r" (ss));
}

__attribute__((always_inline)) inline static void* push_regs(){

    asm volatile("push %ss");
    asm volatile("pushl %esp");
    asm volatile("pushfl");
    asm volatile("push %cs");
    asm volatile("pushl %0" : : "r" (get_eip));
    asm volatile("pushl $0");
    asm volatile("pushl $0");
    asm volatile("pusha");
    asm volatile("push %ds");
}

__attribute__((always_inline)) inline static void pop_context(){

    asm volatile("popl %ebx");
    asm volatile("mov %bx, %ds");
    asm volatile("mov %bx, %es");
    asm volatile("mov %bx, %fs");
    asm volatile("mov %bx, %gs");
        asm volatile("popa");
    asm volatile("addl $0x8, %esp");
    asm volatile("sti");
    asm volatile("iret");
}
     
#endif
