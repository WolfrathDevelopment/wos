/*
 * core.h
 * Wolfrath/Kriewall, 2013
 *
 * Basic io functions and string manipulation
 */

#ifndef CORE_H
#define CORE_H

#include "../types.h"

#define CRTPORT 0x3D4

#define ARRAYSIZE(x) sizeof(x)/sizeof(x[0])

/* io.c */
void out_byte(ushort port, uchar val);
uchar in_byte(ushort port);
ushort in_short(ushort port);
inline uint read_eflags(void);

/* atomic.c */

typedef struct{
	int value;
}Atomic_int;

/* These operations guarenteed to be atomic */
inline int atomic_read(const Atomic_int*);
inline int xchg(volatile uint*,int);
inline int atomic_set(Atomic_int*, int);
inline void atomic_add(Atomic_int*, int);
inline void atomic_sub(Atomic_int*,int);
inline void atomic_inc(Atomic_int*);
inline void atomic_dec(Atomic_int*);

/* console.c */
void printf(char*, ... );
void update_cursor();
void put_char(char);
void put_string(char *);
void put_hex(uint);
void put_decimal(uint);
void clear_screen();

/* string.c */
void memcpy(uchar *, const uchar *, uint);
void memset(uchar *, uchar, uint);
int strcmp(char *, char *);
void strcpy(char *, const char *);
char *strcat(char *, const char *);

/* cpu.c */
typedef struct{
	uint cid;
} Cpu;

inline void cli(void);
inline void sti(void);

/* spinlock.c */
typedef struct{
	uint cpu;
	Atomic_int locked;
}Spinlock;

void init_lock(Spinlock*);
void acquire(Spinlock*);
void release(Spinlock*);
uint is_held(Spinlock*);
void push_cli();
void pop_cli();

#endif
