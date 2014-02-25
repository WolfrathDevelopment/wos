/*
 * task.h
 * Wolfrath/Kriewall, 2013
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
    uint edi;
    uint esi;
    uint ebx;
    uint ebp;
    uint eip;
};
*/

/* Defines a process */

struct w_proc{

	uint pid;
	enum thread_state state;
	enum thread_system sys;
	struct w_proc* next;
	w_pde* pg_dir;
	uint flags;
	struct w_regs* regs;
}__attribute__((packed));


/* pic.c */

typedef void (*w_timer_callback)();

void init_pic();
void reset_pic(uint);
void register_timer(w_timer_callback, uint);


/* proc.c */

void begin_multitasking();
void context_switch(struct w_proc*);
int fork(uint);
int exec();


/* thread.c */

//struct w_thread* create_thread(struct w_proc*, uint);

/* schedule.c */

void schedule();

/* stack.c */

uint get_eip();
inline uint get_esp();
inline void set_esp(uint);
inline void* push_regs();
inline void* copy_stack(uint*,uint*);
inline void pop_context();
inline void set_ss(ushort);

#endif
