/*
 * console.c
 Joel Wolfrath, 2013
 *
 * Impelentation of basic string functions
 */

#include <lib/string.h>
#include <tools/debug.h>

void memcpy(void* dst, const void* src, size_t len)
{
    const uint8_t *sptr = (const uint8_t*) src;
    uint8_t *dptr = (uint8 *)dst;

    while(len--)
    {
        *dptr++ = *sptr++;
    }
}

void memset(uint8_t* dst, uint8_t val, size_t len)
{
    uint8_t* temp = (uint8_t*)dst;
    while(len--)
    {
        *temp++ = val;
    }
}

void memset32(uint32_t* dst, uint32_t val, size_t len)
{
    ASSERT(len % sizeof(uint32) == 0); // Called wrong memset!

	uint32_t* temp = (uint32_t*)dst;
    for(; len != 0; len -= sizeof(uint32_t))
	{
		*temp++ = val;
	}
}

int32_t memcmp(uint8_t* m1, uint8_t* m2, size_t len)
{	
    int32_t i = 0;
    for(; i < len; i++)
    {	
        if(m1[i] != m2[i])
        {
            return 1;
        }
    }

    return 0;
}

void zero(void* dst, size_t len)
{
    if(len % sizeof(uint32_t))
    {
        memset(dst, 0, len);
    }
    else
    {
        memset32(dst, 0, len);
    }
}
