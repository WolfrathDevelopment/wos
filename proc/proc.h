/*
 * task.h
 * Wolfrath/Kriewall, 2013
 *
 */

#ifndef TASK_H
#define TASK_H

/* Models the state of a task at any given time */
typedef struct{
	uint prev_tss;
	uint esp0;
	uint ss0;
	uint esp1;
	uint ss1;
	uint esp2;
	uint ss2;
	uint cr3;
	uint eip;
	uint eflags;
	uint eax;
	uint ecx;
	uint edx;
	uint ebx;
	uint esp;	
	uint ebp;
	uint esi;
	uint edi;
	uint es;
	uint cs;
	uint ss;
	uint ds;
	uint fs;
	uint gs;
	uint ldt;
	ushort trap;
	ushort iomap_base;
} __attribute__((packed)) w_tss;

#endif
