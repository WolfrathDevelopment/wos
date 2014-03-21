/*
 * console.c
 * Joel Wolfrath, 2013
 *
 * Impelentation of console functions
 */

#include "core.h"
#include "../mm/mem.h"

#define COLOR_WHITE 	15
#define COLOR_BLACK		 0

#define NUM_ROWS 		25
#define NUM_COLS 		80

static short cursor_x = 0;
static short cursor_y = 0;

w_uint16 *vid_mem = (w_uint16 *) KVIRT(VGA_FRAME_BUF);

void move_cursor(w_int32 inc, w_int32 x){

	if(x)
		cursor_x += inc;
	else
		cursor_y += inc;

	update_cursor();
}

/* Scroll one line down */

static void scroll(){

	/*
   	 * Low nibble is the foreground color
	 * Upper nibble is the background colour.
	 * Here, background is black and
	 * foreground is white.
	 */

   	w_uint8 attr = (COLOR_BLACK << 4) | (COLOR_WHITE & 0x0F);

   
	/* The attribute byte is the top 8 bits of the short */

	w_uint16 space = ' ' | (attr << 8);

	int i;
	for (i = 0; i < (NUM_ROWS - 1) * NUM_COLS; i++)
		vid_mem[i] = vid_mem[i + NUM_COLS];


	/* Write a blank bottom line */

	for (i = (NUM_ROWS - 1) * NUM_COLS; i < NUM_ROWS * NUM_COLS; i++)
		vid_mem[i] = space;
        
	cursor_y = NUM_ROWS - 1;
}

void update_cursor(){

	if(cursor_x < 0){

		if(cursor_y > 0){
			cursor_x = NUM_COLS - 1;
			cursor_y--;
		}
		else
			cursor_x = 0;
	}
	if(cursor_y < 0)
		cursor_y = 0;

	if(cursor_x > NUM_COLS - 1){
		cursor_x = 0;
		cursor_y++;
	}

	if(cursor_y >= NUM_ROWS)
		scroll();

	/* The screen is 80 characters wide */
	ushort loc = (cursor_y * NUM_COLS) + cursor_x;
	
	/* Signal VGA high byte */
	out_byte(CRTPORT, 14);

	/* Write high byte */
	out_byte(CRTPORT + 1, loc >> 8);

	/* Signal VGA low byte */
	out_byte(CRTPORT, 15);

	/* Write low byte */
	out_byte(CRTPORT + 1, loc);
}

void printf(char* str, ... ){

	w_uint32* argv = (uint*)(w_ptr)(&str + 1);
	int i,c;
	char* s;
	for(i = 0; (c = str[i] & 0xff) != 0; i++){

		if(c != '%'){
			put_char(c);
			continue;
		}

		c = str[++i] & 0xff;
		if(c == 0)
			break;

		switch(c){
		case 'd':
			put_decimal(*argv++);
			break;
		case 'x':
		case 'p':
			put_hex(*argv++);
			break;
		case 's':
			if((s = (char*)*argv++) == 0)
				s = "(null)";
			for(; *s; s++)
				put_char(*s);
			break;
		case '%':
			put_char('%');
			break;
		default:
			put_char('%');
			put_char(c);
			break;
		}
	}
}

void put_char(char c){

	w_uint8 bflag = 0;

	if(c == '\n'){
		cursor_y++;
		cursor_x = 0;
		update_cursor();
		return;
	}
	else if(c == '\b'){
		c = ' ';

		// Move all characters back one space
		int i = cursor_y * NUM_COLS + cursor_x;
		for ( ; i < NUM_ROWS * NUM_COLS; i++)
			vid_mem[i] = vid_mem[i + 1];

		cursor_x--;
		bflag = 1;
	}

	/*
   	 * Low nibble is the foreground color
	 * Upper nibble is the background colour.
	 * Here, background is black (0) and
	 * foreground is white(15).
	 */
   	w_uint8 attr = (COLOR_BLACK << 4) | (COLOR_WHITE & 0x0F);
   
	// The attribute byte is the top 8 bits of the short
   	w_uint16 style = attr << 8;

	w_uint16 *location = vid_mem + (cursor_y * NUM_COLS + cursor_x);
       *location = c | style;

	if(!bflag)
		cursor_x++;
	update_cursor();
}

void put_string(char * str){

	int i = 0;
    	while (str[i])
        	put_char(str[i++]);
}


void put_hex(w_uint32 n){

	int tmp,i;
	//put_string("0x");

	char noZeroes = 1;

	for (i = 28; i > 0; i -= 4){
		tmp = (n >> i) & 0xF;
		if (tmp == 0 && noZeroes != 0){
			put_char('0');
			continue;
		}
    
		if (tmp >= 0xa){
			noZeroes = 0;
			put_char(tmp - 0xA + 'a' );
		}
		else{
			noZeroes = 0;
			put_char( tmp+'0' );
		}
	}
  
	tmp = n & 0xF;
	if (tmp >= 0xA)
		put_char(tmp-0xA+'a');
	else
		put_char(tmp+'0');
}

void put_decimal(w_uint32 n){

	if (n == 0){
		put_char('0');
		return;
	}

	int acc = n;
	int i = 0;
	char c[32];
    
	while (acc > 0){
		c[i] = '0' + acc % 10;
		acc /= 10;
		i++;
	}
	c[i] = 0;

	char c2[32];
	c2[i--] = 0;
	int j = 0;

	while(i >= 0){
		c2[i--] = c[j++];
	}

	put_string(c2);
}

void clear_screen(){

	/* Make an attribute byte for the default colors*/

   	w_uint8 attr = (COLOR_BLACK << 4) | (COLOR_WHITE & 0x0F);
	w_uint16 style = attr << 8;
   	w_uint16 blank = ' ' | style;

   	int i;
   	for (i = 0; i < NUM_ROWS * NUM_COLS; i++)
   	    vid_mem[i] = blank;

   	cursor_x = 0;
   	cursor_y = 0;

	update_cursor();
}
