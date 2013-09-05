/*
 * console.c
 * Wolfrath/Kriewall, 2013
 *
 * Impelentation of console functions
 */

#include "core.h"

ushort *vid_mem = (ushort *)0xB8000;

ushort cursor_x = 0;
ushort cursor_y = 0;

void update_cursor(){

	/* The screen is 80 characters wide */
	ushort loc = (cursor_y * 80) + cursor_x;
	
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

	uint* argv = (uint*)(void*)(&str + 1);
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

	if(c == '\n'){
		cursor_y++;
		cursor_x = 0;
		return;
	}
	
	/*
   	 * Low nibble is the foreground color
	 * Upper nibble is the background colour.
	 * Here, background is black (0) and
	 * foreground is white(15).
	 */
   	uchar attr = (0 << 4) | (15 & 0x0F);
   
	// The attribute byte is the top 8 bits of the short
   	ushort style = attr << 8;

	ushort *location = vid_mem + (cursor_y * 80 + cursor_x);
       *location = c | style;

	cursor_x++;
	update_cursor();
}

void put_string(char * str){

	int i = 0;
    	while (str[i])
        	put_char(str[i++]);
}


void put_hex(uint n){

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

void put_decimal(uint n){

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
	// Make an attribute byte for the default colours
   	uchar attr = (0 << 4) | (15 & 0x0F);
	ushort style = attr << 8;
   	ushort blank = ' ' | style;

   	int i;
   	for (i = 0; i < 80*25; i++)
   	    vid_mem[i] = blank;
   	

   	// Move the hardware cursor back to the start.
   	cursor_x = 0;
   	cursor_y = 0;
   	update_cursor();
}
