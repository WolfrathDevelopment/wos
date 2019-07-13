#include <arch/regs.h>
#include <boot/isr.h>
#include <io/bus.h>
#include <io/console.h>
#include <proc/proc.h>

#define FREQ_FACTOR 	1193180
#define PIC_INTERVAL 	50

static uint32_t count = 0;

struct time_node{

	struct time_node* next;
	uint32_t elapsed;
	uint32_t stop;
};

static void pic_callback(OsIsrFrame* regs){

	count++;
	if( count > 100){
	    count = 0;
	    printf("Schedule Hit");
	    schedule();
	}
}

void init_pic(){

	isr_register(OsIsrPic, &pic_callback);
	reset_pic(PIC_INTERVAL);
}

void reset_pic(uint32_t freq){

	uint32_t div = FREQ_FACTOR / freq;

	write_io_bus_port(0x43, 0x36);
	write_io_bus_port(0x40, (uint8_t)(div & 0xFF));
	write_io_bus_port(0x40, (uint8_t)((div >> 8) & 0xFF));
}

void register_timer(w_timer_callback callback, uint32_t milli){

}
