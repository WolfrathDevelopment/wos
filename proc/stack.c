/*
 * stack.c
 * Wolfrath/Kriewall, 2013
 *
 * Implementation of stack operations
 */

#include "proc.h"

uint get_eip(){

    uint eip;
    asm("movl (%%esp), %0" : "=r"(eip));
    return eip;
}

inline uint get_esp(){

    uint esp;
    asm volatile("movl %%esp, %0" : "=r"(esp));
    return esp;
}

inline void set_esp(uint esp){
    asm volatile("movl %0, %%esp" : : "r" (esp));
}

inline void set_ss(ushort ss){
    asm volatile("mov %0, %%ss" : : "r" (ss));
}

inline void* push_regs(){

    asm volatile("push %ss");
    asm volatile("pushl %esp");
    asm volatile("pushfl");
    asm volatile("push %cs");
    asm volatile("pushl %0" : : "r" (get_eip));
    asm volatile("pushl $0");
    asm volatile("pushl $0");
    asm volatile("pusha");
    asm volatile("push %ds");

    return (void*)get_esp();
}

inline void* copy_stack(uint* ns, uint* cs){

    int i;
    for(i = 0; i < 0x400; i++){
        ns[i] = cs[i];
    }

    uint esp = get_esp();
    return (void*)(((uint)ns) - (((uint)cs) - ((uint)esp)));
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
