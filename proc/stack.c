/*
 * stack.c
 * Joel Wolfrath, 2014
 *
 * Implementation of stack operations
 */

#include <proc/proc.h>

void* get_eip(){

	/* 
	 * Convenient GCC function for getting PC 
	 * God, I love GCC
	 */

	return __builtin_return_address(0);
}

/*
 * If we want the current esp, we better not have
 * stack overhead from a function call...
 */
/*
__attribute__((always_inline)) uint32_t get_esp(){

    uint32_t esp;
    asm volatile("movl %%esp, %0" : "=r"(esp));
    return esp;
}

__attribute__((always_inline)) void set_esp(uint32_t esp){
    asm volatile("movl %0, %%esp" : : "r" (esp));
}

__attribute__((always_inline)) void set_ss(uint16 ss){
    asm volatile("mov %0, %%ss" : : "r" (ss));
}

__attribute__((always_inline)) void* push_regs(){

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
*/
void* copy_stack(uint32_t* ns, uint32_t* cs, uint32_t esp){

    int i = 0;
    /* TODO eww constants... */
    for(i = 0; i < 0x400; i++){
        ns[i] = cs[i];
    }

    return (void*)(((uint32_t)ns) + (esp - ((uint32_t)cs)));
}
/*
__attribute__((always_inline)) void pop_context(){

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
*/
