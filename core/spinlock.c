/*
 * spinlock.c
 * Joel Wolfrath, 2013
 *
 * Impelentation of spinlock functions
 */

#include "core.h"

void init_lock(struct w_spinlock* lk){

	atomic_set(&lk->locked,0);
	lk->cpu = 0;
}

void acquire(struct w_spinlock* lk){

	// prevent deadlock
	push_cli();

	if(is_held(lk))
		return;
	
	//aquire the lock
	while(atomic_set(&lk->locked,1) != 0)
		;

	//set actual cpu that aquired the lock
	//lk->cpu = current_cpu
}

void release(struct w_spinlock* lk){

	if(!is_held(lk))
		return;

	lk->cpu = 0;
	atomic_set(&lk->locked,0);

	pop_cli();
}

w_uint32 is_held(struct w_spinlock* lk){

	return atomic_read(&lk->locked);
	//make sure the running cpu is the one holding the lock
	//return lk->locked && lk->cpu == current_cpu
}

//these will eventually perform more cpu specific functions
void push_cli(){
	cli();
}

void pop_cli(){
	sti();
}
