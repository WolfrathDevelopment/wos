
#include "proc.h"

extern struct w_proc current_proc;
extern struct w_thread current_thread;

extern void perform_context_switch();

void schedule(){

	/* Thread stack has already been saved from common ISR handler */
	perform_context_switch();
}
