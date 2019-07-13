#ifndef __OS_BUS_H
#define __OS_BUS_H

/* IO Bus Registers */

#define IO_BUS_MASTER_PIC_CONTROL_REG   (0x0020)
#define IO_BUS_MASTER_PIC_DATA_REG      (0x0021)
#define IO_BUS_KBD_SCANCODE_REG         (0x0060)
#define IO_BUS_SLAVE_PIC_CONTROL_REG    (0x00A0)
#define IO_BUS_SLAVE_PIC_DATA_REG       (0x00A1)
#define IO_BUS_VGA_CONTROL_REG          (0x03D4)
#define IO_BUS_VGA_DATA_REG             (0x03D5)

#define IO_BUS_HIGH_BYTE                (14)
#define IO_BUS_LOW_BYTE                 (15)

void write_io_bus_port(uint16_t port, uint8_t value);
uint8_t read_io_bus(uint16_t port);

#endif /* __OS_BUS_H */
