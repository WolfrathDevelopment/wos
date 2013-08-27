#ifndef CORE_H
#define CORE_H

#include "../types.h"

#define CRTPORT 0x3D4

#define ARRAYSIZE(x) sizeof(x) / sizeof(x[0])

/* Write a byte to the specified port */
void out_byte(ushort port, uchar val);

/* Read a byte from the specified port */
uchar in_byte(ushort port);

/* Read two bytes from the specified port */
ushort in_short(ushort port);

/* Set the cursor position */
void update_cursor();
void put_char(char);
void put_string(char *);
void clear_screen();

#endif
