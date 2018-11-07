#include <types.h>
#include <mm/mem.h>
#include <io/mmio.h>
#include <lib/string.h>

static uint8_t* MmioAddressMap[MmioMaxDevices] = {

    (uint8*) NULL,          /* Invalid device */
    (uint8*) 0x000B8000     /* VGA frame buffer (console) */
};

OsRc mmio_write_bytes(MmioDevice device, uint8_t* data, uint32_t offset, size_t length)
{
    OsRc rc = RC_SUCCESS;

    if(!data ||
       !length ||
       device == MmioInvalid ||
       device >= MmioMaxDevices)
    {
        rc = RC_INVALID_PARM;
    }
    else
    {
        uint8_t* mmioPhysAddress = MmioAddressMap[device];
        if(mmioPhysAddress)
        {
            uint8_t* mmioVirtAddress = KVIRT(mmioPhysAddress);
            memcpy(mmioVirtAddress + offset, data, length);
        }
    }

    return rc;
}
