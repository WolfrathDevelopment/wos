/*
 * core.h
 * Joel Wolfrath, 2013
 *
 * Basic io functions and string manipulation
 */

#ifndef CORE_H
#define CORE_H

#include "../types.h"

#define CRTPORT 			0x3D4
#define VGA_FRAME_BUF 		0xB8000
#define DEFINE_SPINLOCK(x) 	struct w_spinlock x;
#define NUM_CPU 			4

/* io.c */

void zero_mem(w_ptr, w_uint32);
void out_byte(w_uint16 port, w_uint8 val);
w_uint8 in_byte(w_uint16 port);
w_uint16 in_short(w_uint16 port);
inline w_uint32 read_eflags(void);
inline void cli(void);
inline void sti(void);


/* atomic.c */

struct w_atomic{
	w_int32 value;
};

/* These operations guarenteed to be atomic */
inline w_int32 atomic_read(const struct w_atomic*);
inline w_int32 xchg(volatile w_uint32*,w_int32);
inline w_int32 atomic_set(struct w_atomic*, w_int32);
inline void atomic_add(struct w_atomic*, w_int32);
inline void atomic_sub(struct w_atomic*,w_int32);
inline void atomic_inc(struct w_atomic*);
inline void atomic_dec(struct w_atomic*);


/* console.c */

void move_cursor(w_int32, w_int32);
void printf(char*, ... );
void update_cursor();
void put_char(char);
void put_string(char *);
void put_hex(w_uint32);
void put_decimal(w_uint32);
void clear_screen();


/* string.c */

void memcpy(w_uint8 *, const w_uint8 *, w_uint32);
void memset(w_uint8 *, w_uint8, w_uint32);
int memcmp(w_uint8*, w_uint8*,w_uint32);
w_int32 strcmp(char *, char *);
void strcpy(char *, const char *);
char *strcat(char *, const char *);

/* spinlock.c */
struct w_spinlock{
	w_uint32 cpu;
	struct w_atomic locked;
};

void init_lock(struct w_spinlock*);
void acquire(struct w_spinlock*);
void release(struct w_spinlock*);
w_uint32 is_held(struct w_spinlock*);
void push_cli();
void pop_cli();

#endif
