#include <types.h>
#include <io/mmio.h>
#include <lib/string.h>

static uint8_t* MmioAddressMap[MaxDevices] = {

    (uint8*) NULL,          /* Invalid device */
    (uint8*) 0x000B8000     /* VGA frame buffer (console) */
};

OsRc write_device_bytes(MmioDevice device, uint8_t* data, uint32_t offset, size_t length)
{
    OsRc rc = RC_SUCCESS;

    if(!data ||
       !length ||
       device == Invalid ||
       device >= MaxDevices)
    {
        rc = RC_INVALID_PARM;
    }
    else
    {
        uint8_t* mmioAddress = MmioAddressMap[device];
        if(mmioAddress)
        {
            memcpy(mmioAddress + offset, data, length);
        }
    }

    return rc;
}
