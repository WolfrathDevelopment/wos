#ifndef CORE_H
#define CORE_H

#include "../types.h"

#define CRTPORT 0x3D4

#define ARRAYSIZE(x) sizeof(x)/sizeof(x[0])

/* io.c */
void out_byte(ushort port, uchar val);
uchar in_byte(ushort port);
ushort in_short(ushort port);

/* console.c */
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

#endif
