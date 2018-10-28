#ifndef __OS_MMIO_H
#define __OS_MMIO_H

typedef enum {

  Invalid     = 0x00,
  Console     = 0x01,
  MaxDevices  = 0x02

} MmioDevice;

int write_device_bytes(MmioDevice device, uint8* data, uint32 offset, uint32 length);

#endif /* __OS_MMIO_H */
