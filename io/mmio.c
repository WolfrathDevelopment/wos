#include <types.h>
#include <io/mmio.h>
#include <lib/string.h>

static uint8* MmioAddressMap[MaxDevices] = {
  (uint8*) NULL,          /* Invalid device */
  (uint8*) 0x000B8000     /* VGA frame buffer (console) */
};

int write_device_bytes(MmioDevice device, uint8* data, uint32 offset, uint32 length)
{
  int rc = 0;

  if(!data)
  {
    rc = 1;
  }
  else if(!length)
  {
    rc = 2;
  }
  else if(device == Invalid || device >= MaxDevices)
  {
    rc = 3;
  }
  else
  {
    uint8* mmioAddress = MmioAddressMap[device];
    if(mmioAddress)
    {
      memcpy(mmioAddress + offset, data, length);
    }
  }

  return rc;
}
