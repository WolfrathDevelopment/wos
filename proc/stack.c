/*
 * stack.c
 * Joel Wolfrath, 2013
 *
 * Implementation of stack operations
 */

#include "proc.h"

w_uint32 get_eip(){

    w_uint32 eip;
    asm("movl (%%esp), %0" : "=r"(eip));
    return eip;
}

inline w_uint32 get_esp(){

    w_uint32 esp;
    asm volatile("movl %%esp, %0" : "=r"(esp));
    return esp;
}

inline void set_esp(w_uint32 esp){
    asm volatile("movl %0, %%esp" : : "r" (esp));
}

inline void set_ss(w_uint16 ss){
    asm volatile("mov %0, %%ss" : : "r" (ss));
}

inline w_ptr push_regs(){

    asm volatile("push %ss");
    asm volatile("pushl %esp");
    asm volatile("pushfl");
    asm volatile("push %cs");
    asm volatile("pushl %0" : : "r" (get_eip));
    asm volatile("pushl $0");
    asm volatile("pushl $0");
    asm volatile("pusha");
    asm volatile("push %ds");

    return (w_ptr)get_esp();
}

inline w_ptr copy_stack(w_uint32* ns, w_uint32* cs){

    int i;
    for(i = 0; i < 0x400; i++){
        ns[i] = cs[i];
    }

    w_uint32 esp = get_esp();
    return (w_ptr)(((w_uint32)ns) - (((w_uint32)cs) - ((w_uint32)esp)));
}

inline void pop_context(){

    asm volatile("popl %ebx");
    asm volatile("mov %bx,%ds");
    asm volatile("mov %bx,%es");
    asm volatile("mov %bx,%fs");
    asm volatile("mov %bx,%gs");
    asm volatile("popa");
    asm volatile("addl $0x8, %esp");
    asm volatile("sti");
    asm volatile("iret");
}
