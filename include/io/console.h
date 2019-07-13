/*
 * console.h
 * Joel Wolfrath, 2013
 *
 * Functions to manage the console
 */

#ifndef _OS_CONSOLE_H
#define _OS_CONSOLE_H

#include <types.h>

void console_init();

void console_notify_key_pressed(uint8_t scancode);

void printf(char* fmt, ... );

// TODO REMOVE THIS and use the console_notify()
void update_cursor();
void put_char(char);
void move_cursor(int32_t, int32_t);
void put_decimal(uint32_t);
void put_hex(uint32_t);
#endif /* _OS_CONSOLE_H */
