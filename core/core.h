/*
 * core.h
 * Wolfrath/Kriewall, 2013
 *
 * Basic io functions and string manipulation
 */

#ifndef CORE_H
#define CORE_H

#include "../types.h"

#define CRTPORT 		0x3D4
#define VGA_FRAME_BUF 		0xB8000
#define DEFINE_SPINLOCK(x) 	struct w_spinlock x;
#define NUM_CPU 		4

/* io.c */

void zero_mem(void*, uint);
void out_byte(ushort port, uchar val);
uchar in_byte(ushort port);
ushort in_short(ushort port);
inline uint read_eflags(void);
inline void cli(void);
inline void sti(void);


/* atomic.c */

struct w_atomic{
	int value;
};

/* These operations guarenteed to be atomic */
inline int atomic_read(const struct w_atomic*);
inline int xchg(volatile uint*,int);
inline int atomic_set(struct w_atomic*, int);
inline void atomic_add(struct w_atomic*, int);
inline void atomic_sub(struct w_atomic*,int);
inline void atomic_inc(struct w_atomic*);
inline void atomic_dec(struct w_atomic*);


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

/* spinlock.c */
struct w_spinlock{
	uint cpu;
	struct w_atomic locked;
};

void init_lock(struct w_spinlock*);
void acquire(struct w_spinlock*);
void release(struct w_spinlock*);
uint is_held(struct w_spinlock*);
void push_cli();
void pop_cli();

#endif
