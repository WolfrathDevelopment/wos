/*
 * proc.h
 * Joel Wolfrath, 2013
 *
 */

#ifndef PROC_H
#define PROC_H

#include <mm/mem.h>
#include <mm/paging.h>
#include <boot/boot.h>
#include <types.h>
#include <lib/core.h>


/* What is the state of the given thread? */

enum thread_state {CREATED, RUNABLE, RUNNING, WAITING, SLEEPING, STOPPED};
typedef enum thread_state TaskState;

/*
struct context {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebx;
    uint32_t ebp;
    uint32_t eip;
};
*/

/* Defines a process */

struct w_proc{

	uint32_t          pid;
	enum thread_state state;
	struct w_proc*    next;
	PageDirectory*    pg_dir;
	uint32_t          flags;
	OsIsrFrame*       regs;
} PACKED;

struct _Task
{
    uint32_t            pid;
    TaskState           state;
    PageDirectory*      page_directory;
    uint32_t            flags;
    OsIsrFrame*         registers;
};
typedef struct _Task Task;

Task* get_current_task();

/* pic.c */

typedef void (*w_timer_callback)();

void init_pic();
void reset_pic(uint32_t);
void register_timer(w_timer_callback, uint32_t);


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
void* copy_stack(uint32_t*, uint32_t*,uint32_t);

/*
 * If we want the current esp, we better not have
 * stack overhead from a function call...
 */

FORCE_INLINE static uint32_t get_esp(){

    uint32_t esp;
    asm volatile("movl %%esp, %0" : "=r"(esp));
    return esp;
}

FORCE_INLINE static void set_esp(uint32_t esp){
    asm volatile("movl %0, %%esp" : : "r" (esp));
}

FORCE_INLINE static void set_ss(uint16_t ss){
    asm volatile("mov %0, %%ss" : : "r" (ss));
}

FORCE_INLINE static void* push_regs(){

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

FORCE_INLINE static void pop_context(){

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
