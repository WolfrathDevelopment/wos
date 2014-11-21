
#include "proc.h"

extern struct w_proc* current_proc;

void schedule(){

	/* Thread stack has already been saved from common ISR handler */

    //current_proc = current_proc->next;
	//if(current_proc->state == RUNABLE){
    //context_switch(current_proc);
	//}
}
