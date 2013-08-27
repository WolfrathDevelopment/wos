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

void put_char(char c){
	
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
