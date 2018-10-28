#ifndef __OS_BUS_H
#define __OS_BUS_H

/* IO Bus Registers */
#define IO_BUS_VGA_CONTROL_REG  (0x03D4)
#define IO_BUS_VGA_DATA_REG     (0x03D5)

void write_io_bus_port(uint16 port, uint8 value);
uint8 read_io_bus(uint16 port);

#endif /* __OS_BUS_H */
