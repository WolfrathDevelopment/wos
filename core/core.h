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
#define VGA_FRAME_BUF 0xB8000
#define DEFINE_SPINLOCK(x) w_spinlock x;

/* io.c */
void zero_mem(uint*, uint);
void out_byte(ushort port, uchar val);
uchar in_byte(ushort port);
ushort in_short(ushort port);
inline uint read_eflags(void);

/* atomic.c */

typedef struct{
	int value;
}w_atomic;

/* These operations guarenteed to be atomic */
inline int atomic_read(const w_atomic*);
inline int xchg(volatile uint*,int);
inline int atomic_set(w_atomic*, int);
inline void atomic_add(w_atomic*, int);
inline void atomic_sub(w_atomic*,int);
inline void atomic_inc(w_atomic*);
inline void atomic_dec(w_atomic*);

/* console.c */
void move_cursor(int,int);
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
} w_cpu;

inline void cli(void);
inline void sti(void);

/* spinlock.c */
typedef struct{
	uint cpu;
	w_atomic locked;
}w_spinlock;

void init_lock(w_spinlock*);
void acquire(w_spinlock*);
void release(w_spinlock*);
uint is_held(w_spinlock*);
void push_cli();
void pop_cli();

#endif
