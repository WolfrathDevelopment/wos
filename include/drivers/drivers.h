/*
 * drivers.h
 * Joel Wolfrath, 2013
 * 
 * Functions associated with device drivers
 */

#include <lib/core.h>
#include <boot/boot.h>

#ifndef DRIVERS_H
#define DRIVERS_H

/* Special keyboard scancodes */

#define KBD_SC_CAPS_LOCK    (58)
#define KBD_SC_BACKSPACE    (14)
#define KBD_SC_LEFT_SHIFT   (42)
#define KBD_SC_RIGHT_SHIFT  (54)
#define KBD_SC_LEFT_ARROW   (75)
#define KBD_SC_RIGHT_ARROW  (77)
#define KBD_SC_UP_ARROW     (72)
#define KBD_SC_DOWN_ARROW   (80)
#define KBD_SC_DELETE_KEY   (83)
#define KBD_SC_ENTER_KEY    (28)

#define KBD_KEY_RELEASE_MASK  (0x80)

/* kbd.c */

void kbd_install();

#endif
