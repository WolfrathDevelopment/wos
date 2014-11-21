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
    w_uint32 edi;
    w_uint32 esi;
    w_uint32 ebx;
    w_uint32 ebp;
    w_uint32 eip;
};
*/

/* Defines a process */

struct w_proc{

	w_uint32 pid;
	enum thread_state state;
	enum thread_system sys;
	struct w_proc* next;
	w_pde* pg_dir;
	w_uint32 flags;
	struct w_regs* regs;
}__attribute__((packed));


/* pic.c */

typedef void (*w_timer_callback)();

void init_pic();
void reset_pic(w_uint32);
void register_timer(w_timer_callback, w_uint32);


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

w_ptr get_eip();
inline w_uint32 get_esp();
inline void set_esp(w_uint32);
inline w_ptr push_regs();
w_ptr copy_stack(w_uint32*, w_uint32*,w_uint32);
inline void pop_context();
inline void set_ss(w_uint16);

#endif
