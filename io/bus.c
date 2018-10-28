#include <types.h>
#include <io/bus.h>

void write_io_bus_port(uint16 port, uint8 value)
{
  asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

uint8 read_io_bus(uint16 port)
{
  uint8 value;
  asm volatile("inb %1, %0" : "=a" (value) : "dN" (port));
  return value;
}
