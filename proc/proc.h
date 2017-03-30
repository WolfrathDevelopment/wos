/*
 * proc.h
 * Joel Wolfrath, 2013
 *
 */

#ifndef PROC_H
#define PROC_H

#include "../mm/mem.h"
#include "../boot/boot.h"
#include "../types.h"
#include "../core/core.h"


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
inline uint32 get_esp();
inline void set_esp(uint32);
inline void* push_regs();
void* copy_stack(uint32*, uint32*,uint32);
inline void pop_context();
inline void set_ss(uint16);

#endif
