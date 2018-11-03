#include <types.h>
#include <io/bus.h>

void write_io_bus_port(uint16_t port, uint8_t value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

uint8_t read_io_bus(uint16_t port)
{
    uint8_t value;
    asm volatile("inb %1, %0" : "=a" (value) : "dN" (port));
    return value;
}
