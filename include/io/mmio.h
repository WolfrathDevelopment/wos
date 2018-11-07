#include <lib/string.h>

#ifndef __OS_MMIO_H
#define __OS_MMIO_H

typedef enum {

    MmioInvalid     = 0x00,
    MmioConsole     = 0x01,
    MmioMaxDevices  = 0x02

} MmioDevice;

OsRc mmio_write_bytes(MmioDevice device, uint8* data, uint32 offset, uint32 length);

#endif /* __OS_MMIO_H */
