/*
 * stack.c
 * Joel Wolfrath, 2014
 *
 * Implementation of stack operations
 */

#include "proc.h"

w_ptr get_eip(){

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

__attribute__((always_inline)) inline w_uint32 get_esp(){

    w_uint32 esp;
    asm volatile("movl %%esp, %0" : "=r"(esp));
    return esp;
}

__attribute__((always_inline)) inline void set_esp(w_uint32 esp){
    asm volatile("movl %0, %%esp" : : "r" (esp));
}

__attribute__((always_inline)) inline void set_ss(w_uint16 ss){
    asm volatile("mov %0, %%ss" : : "r" (ss));
}

__attribute__((always_inline)) inline w_ptr push_regs(){

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

w_ptr copy_stack(w_uint32* ns, w_uint32* cs, w_uint32 esp){

    int i;
    for(i = 0; i < 0x400; i++){
        ns[i] = cs[i];
    }

    return (w_ptr)(((w_uint32)ns) + (esp - ((w_uint32)cs)));
}

__attribute__((always_inline)) inline void pop_context(){

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
