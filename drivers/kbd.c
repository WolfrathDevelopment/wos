/*
 * kbd.c
 Joel Wolfrath, 2013
 *
 * Implementation of keyboard functions
 */

#include <drivers/drivers.h>

#define CAPS_LOCK 		58
#define BACKSPACE 		14
#define LEFT_SHIFT		42
#define RIGHT_SHIFT 	54
#define LEFT_ARROW 		75
#define RIGHT_ARROW 	77
#define UP_ARROW 		72
#define DOWN_ARROW 		80
#define DELETE_KEY 		83
#define ENTER_KEY 		28

#define X_DIR 			1
#define Y_DIR 			0

static uint16 _shift=0;
static uint16 c_lock = 0;

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


static void kbd_callback(Registers regs){

	uint8 scancode = in_byte(0x60);

	if(scancode < 0)
		return;

	if(scancode & 0x80){

		/* key was released, adjust scancode */

		scancode -= 0x80;

		if(scancode == LEFT_SHIFT || scancode == RIGHT_SHIFT)
			_shift=0;
		else if(scancode == CAPS_LOCK && c_lock == 2)
			c_lock = 0;
	}
	else{

		/* key was pressed */

		switch(scancode){

		case DELETE_KEY:

			put_char(' ');
			move_cursor(-1, X_DIR);
			break;

		case LEFT_ARROW:

			move_cursor(-1, X_DIR);
			break;

		case RIGHT_ARROW:

			move_cursor(1, X_DIR);
			break;

		case DOWN_ARROW:

			move_cursor(1, Y_DIR);
			break;

		case UP_ARROW:

			move_cursor(-1, Y_DIR);
			break;

		case LEFT_SHIFT:
		case RIGHT_SHIFT:

			_shift = 1;
			break;

		case CAPS_LOCK:
			
			c_lock++;
			break;

		default:

			if(_shift || c_lock)
				put_char(skbdus[scancode]);
			else
				put_char(kbdus[scancode]);
			break;
		}
	}
}

void kbd_install(){

	register_isr(IRQ1, &kbd_callback);
}
