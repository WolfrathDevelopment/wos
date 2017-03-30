/*
 * core.h
 * Joel Wolfrath, 2013
 *
 * Basic io functions and string manipulation
 */

#ifndef CORE_H
#define CORE_H

#include "../types.h"

#define CRTPORT 			(0x3D4)
#define VGA_FRAME_BUF 		(0xB8000)

static inline void * alignAddress(void * addr, uint32 len) {

    uint32 alignedAddr = (uint32) addr;
    uint32 mod = alignedAddr % len;

    if(mod){
        alignedAddr = (alignedAddr - mod) + len;
    }

    return (void *) alignedAddr;
}

/* io.c */

void zero(void*, size_t);
void out_byte(uint16 port, uint8 val);
uint8 in_byte(uint16 port);
uint16 in_short(uint16 port);
inline uint32 read_eflags(void);
inline void cli(void);
inline void sti(void);

/* console.c */

void move_cursor(int32, int32);
void printf(char*, ... );
void update_cursor();
void put_char(char);
void put_string(char *);
void put_hex(uint32);
void put_decimal(uint32);
void clear_screen();


/* string.c */

void memcpy(uint8 *, const uint8 *, size_t);
void memset(uint8 *, uint8, size_t);
void memset32(uint32 *, uint32, size_t);
int memcmp(uint8*, uint8*, size_t);
int32 strcmp(char *, char *);
void strcpy(char *, const char *);
char *strcat(char *, const char *);

#endif
