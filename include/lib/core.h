/*
 * core.h
 * Joel Wolfrath, 2013
 *
 * Basic io functions and string manipulation
 */

#ifndef CORE_H
#define CORE_H

#include <types.h>

#define CRTPORT			(0x000003D4)
#define VGA_FRAME_BUF 		(0x000B8000)

static inline void * alignAddress(void * addr, uint32 len) {

    uint32 alignedAddr = (uint32) addr;
    uint32 mod = alignedAddr % len;

    if(mod){
        alignedAddr = (alignedAddr - mod) + len;
    }

    return (void *) alignedAddr;
}

/* io.c */

void out_byte(uint16 port, uint8 val);
uint8 in_byte(uint16 port);
uint16 in_short(uint16 port);

inline uint32 read_eflags(void)
{
	uint32 eflags;
	asm volatile("pushfl; popl %0" : "=r" (eflags));
	return eflags;	
}

static inline void cli(void)
{
	asm volatile("cli");
}

static inline void sti(void)
{
	asm volatile("sti");
}

/* console.c */

void move_cursor(int32, int32);
void printf(char*, ... );
void update_cursor();
void put_char(char);
void put_string(char *);
void put_hex(uint32);
void put_decimal(uint32);
void clear_screen();

#endif
