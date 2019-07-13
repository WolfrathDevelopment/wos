#include <lib/string.h>

#ifndef __OS_MMIO_H
#define __OS_MMIO_H

typedef enum {

    MmioInvalid     = 0x00,
    MmioConsole     = 0x01,
    MmioMaxDevices  = 0x02

} MmioDevice;

OsRc mmio_read_bytes(MmioDevice device, uint8_t* dst, uint32_t offset, uint32_t length);
OsRc mmio_write_bytes(MmioDevice device, uint8_t* data, uint32_t offset, uint32_t length);

#endif /* __OS_MMIO_H */
