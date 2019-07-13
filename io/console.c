/*
 * console.c
 * Joel Wolfrath, 2013
 *
 * Impelentation of console functions
 */

#include <lib/core.h>
#include <mm/mem.h>
#include <io/bus.h>
#include <io/mmio.h>
#include <io/console.h>

#define COLOR_WHITE 	15
#define COLOR_BLACK	 0

#define NUM_ROWS 	25
#define NUM_COLS 	80

static short cursor_x = 0;
static short cursor_y = 0;

void move_cursor(int32_t inc, int32_t x){

	if(x)
		cursor_x += inc;
	else
		cursor_y += inc;

	update_cursor();
}

static void write_styled_char(uint16_t character, uint32_t index)
{
    uint32_t writeOffset = index * sizeof(uint16_t);
    mmio_write_bytes(MmioConsole, (uint8_t*)&character, writeOffset, sizeof(character));
}

static uint16_t read_styled_char(uint32_t index)
{
    uint16_t returnValue = 0;
    uint32_t readOffset = index * sizeof(uint16_t);
    mmio_read_bytes(MmioConsole, (uint8_t*)&returnValue, readOffset, sizeof(returnValue));

    return returnValue;
}

/* Scroll one line down */

static void scroll(){

	/*
   	 * Low nibble is the foreground color
	 * Upper nibble is the background colour.
	 * Here, background is black and
	 * foreground is white.
	 */

   	uint8_t attr = (COLOR_BLACK << 4) | (COLOR_WHITE & 0x0F);

   
	/* The attribute byte is the top 8 bits of the short */
	uint16_t space = ' ' | (attr << 8);

	int i;
	for (i = 0; i < (NUM_ROWS - 1) * NUM_COLS; i++)
    {
        uint32_t readOffset = (i + NUM_COLS) * sizeof(uint16_t);
        uint32_t writeOffset = i * sizeof(uint16_t);
        uint16_t value = space;

        write_styled_char(read_styled_char(i + NUM_COLS), i);
    }

	/* Write a blank bottom line */
	for (i = (NUM_ROWS - 1) * NUM_COLS; i < NUM_ROWS * NUM_COLS; i++)
    {
        write_styled_char(space, i);
    }
    
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
	uint16_t loc = (cursor_y * NUM_COLS) + cursor_x;
	
	/* Signal VGA high byte */
	write_io_bus_port(IO_BUS_VGA_CONTROL_REG, 14);

	/* Write high byte */
	write_io_bus_port(IO_BUS_VGA_DATA_REG, loc >> 8);

	/* Signal VGA low byte */
	write_io_bus_port(IO_BUS_VGA_CONTROL_REG, 15);

	/* Write low byte */
	write_io_bus_port(IO_BUS_VGA_DATA_REG, loc);
}

void printf(char* str, ... ){

	uint32_t* argv = (uint32_t*)(void*)(&str + 1);
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

	uint8_t bflag = 0;

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
        {
            write_styled_char(read_styled_char(i + 1), i); 
        }

		cursor_x--;
		bflag = 1;
	}

	/*
   	 * Low nibble is the foreground color
	 * Upper nibble is the background colour.
	 * Here, background is black (0) and
	 * foreground is white(15).
	 */
   	uint8_t attr = (COLOR_BLACK << 4) | (COLOR_WHITE & 0x0F);
   
	// The attribute byte is the top 8 bits of the short
   	uint16_t styledChar = (attr << 8) | c;

    write_styled_char(styledChar, cursor_y * NUM_COLS + cursor_x);

	if(!bflag)
		cursor_x++;
	update_cursor();
}

void put_string(char * str){

	int i = 0;
    	while (str[i])
        	put_char(str[i++]);
}


void put_hex(uint32_t n){

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

void put_decimal(uint32_t n){

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

static void clear_screen()
{
	/* Make an attribute byte for the default colors*/

   	uint8_t attr = (COLOR_BLACK << 4) | (COLOR_WHITE & 0x0F);
	uint16_t style = attr << 8;
   	uint16_t blank = ' ' | style;

   	int i;
   	for (i = 0; i < NUM_ROWS * NUM_COLS; i++)
    {
        write_styled_char(blank, i);
    }

   	cursor_x = 0;
   	cursor_y = 0;

	update_cursor();
}

void console_init()
{
    clear_screen();
}
