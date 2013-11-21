
#include "proc.h"

#define FREQ_FACTOR 1193180
#define PIC_INTERVAL 50

static uint count = 0;

struct time_node{

	struct time_node* next;
	uint elapsed;
	uint stop;
};

static void pic_callback(struct w_regs regs){

	count++;
	if( count > 100){
	    count = 0;
	    printf("Schedule Hit");
	    //schedule();
	}
}

void init_pic(){

	register_interrupt_handler(INT_PIC, &pic_callback);
	reset_pic(PIC_INTERVAL);
}

void reset_pic(uint freq){

	uint div = FREQ_FACTOR / freq;

	out_byte(0x43, 0x36);
	out_byte(0x40, (uchar)(div & 0xFF));
	out_byte(0x40, (uchar)((div >> 8) & 0xFF));
}

void register_timer(w_timer_callback callback, uint milli){

}
