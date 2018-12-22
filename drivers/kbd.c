/*
 * kbd.c
 * Joel Wolfrath, 2013
 *
 * Implementation of keyboard functions
 */

#include <arch/regs.h>
#include <boot/isr.h>
#include <drivers/drivers.h>
#include <io/bus.h>

#define X_DIR 			1
#define Y_DIR 			0

static uint32 SHIFT_ENABLED = 0;
static uint32 CAPS_LOCK_PRESS_COUNT = 0;

/* standard scancodes */

static uint8 kbdus[128] ={

    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */

  '9', '0', '-', '=', '\b',	/* Backspace */

  '\t',			/* Tab */

  'q', 'w', 'e', 'r',	/* 19 */

  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */

    0,			/* 29   - Control */

  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */

 '\'', '`',   0,		/* Left shift */

 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */

  'm', ',', '.', '/',   0,				/* Right shift */

  '*',

    0,	/* Alt */

  ' ',	/* Space bar */

    0,	/* Caps lock */

    0,	/* 59 - F1 key ... > */

    0,   0,   0,   0,   0,   0,   0,   0,

    0,	/* < ... F10 */

    0,	/* 69 - Num lock*/

    0,	/* Scroll Lock */

    0,	/* Home key */

    0,	/* Up Arrow */

    0,	/* Page Up */

  '-',

    0,	/* Left Arrow */

    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/

    0,	/* Down Arrow */

    0,	/* Page Down */

    0,	/* Insert Key */

    0,	/* Delete Key */

    0,   0,   0,

    0,	/* F11 Key */

    0,	/* F12 Key */

    0,	/* All other keys are undefined */
};

/* shift scancodes */

static uint8 skbdus[128] ={

    0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */

  '(', ')', '_', '+', '\b',	/* Backspace */

  '\t',			/* Tab */

  'Q', 'W', 'E', 'R',	/* 19 */

  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',	/* Enter key */

    0,			/* 29   - Control */

  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',	/* 39 */

 '\"', '~',   0,		/* Left shift */

 '|', 'Z', 'X', 'C', 'V', 'B', 'N',			/* 49 */

  'M', '<', '>', '?',   0,				/* Right shift */

  '*',

    0,	/* Alt */

  ' ',	/* Space bar */

    0,	/* Caps lock */

    0,	/* 59 - F1 key ... > */

    0,   0,   0,   0,   0,   0,   0,   0,

    0,	/* < ... F10 */

    0,	/* 69 - Num lock*/

    0,	/* Scroll Lock */

    0,	/* Home key */

    0,	/* Up Arrow */

    0,	/* Page Up */

  '-',

    0,	/* Left Arrow */

    0,

    0,	/* Right Arrow */

  '+',

    0,	/* 79 - End key*/

    0,	/* Down Arrow */

    0,	/* Page Down */

    0,	/* Insert Key */

    0,	/* Delete Key */

    0,   0,   0,

    0,	/* F11 Key */

    0,	/* F12 Key */

    0,	/* All other keys are undefined */
};

static void kbd_callback(OsIsrFrame* regs){

  uint8_t scancode = read_io_bus(IO_BUS_KBD_SCANCODE_REG);

  if(scancode & KBD_KEY_RELEASE_MASK) {

    /* key was released, get original value */
    scancode &= ~KBD_KEY_RELEASE_MASK;

    if(scancode == KBD_SC_LEFT_SHIFT ||
       scancode == KBD_SC_RIGHT_SHIFT) {

      SHIFT_ENABLED = FALSE;
    }
    else if(scancode == KBD_SC_CAPS_LOCK && 
            CAPS_LOCK_PRESS_COUNT == 2) {

      CAPS_LOCK_PRESS_COUNT = 0;
    }
  }
  else{

    /* key was pressed */

    switch(scancode){

    case KBD_SC_DELETE_KEY:

      put_char(' ');
      move_cursor(-1, X_DIR);
      break;

    case KBD_SC_LEFT_ARROW:

      move_cursor(-1, X_DIR);
      break;

    case KBD_SC_RIGHT_ARROW:

      move_cursor(1, X_DIR);
      break;

    case KBD_SC_DOWN_ARROW:

      move_cursor(1, Y_DIR);
      break;

    case KBD_SC_UP_ARROW:

      move_cursor(-1, Y_DIR);
      break;

    case KBD_SC_LEFT_SHIFT:
    case KBD_SC_RIGHT_SHIFT:

      SHIFT_ENABLED = TRUE;	
      break;

    case KBD_SC_CAPS_LOCK:
			
      CAPS_LOCK_PRESS_COUNT++;
      break;

    default:

      if(SHIFT_ENABLED ||
         CAPS_LOCK_PRESS_COUNT > 0) {

        put_char(skbdus[scancode]);
      }
      else {
        put_char(kbdus[scancode]);
      }
      break;
    }
  }
}

void kbd_install(){

	isr_register(OsIsrIrq1, &kbd_callback);
}
