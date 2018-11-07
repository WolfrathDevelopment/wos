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

#endif /* _OS_CONSOLE_H */
