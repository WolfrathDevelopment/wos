/*
 * console.c
 Joel Wolfrath, 2013
 *
 * Impelentation of basic string functions
 */

#include <lib/string.h>
#include <lib/debug.h>

void memcpy(uint8 *dest, const uint8 *src, size_t len){

	const uint8 *sp = (const uint8 *)src;
	uint8 *dp = (uint8 *)dest;

	while(len--)
		*dp++ = *sp++;
}

void memset(uint8 *dst, uint8 val, size_t len)
{
	ASSERT(dst);

	uint8 *temp = (uint8 *)dst;
	while(len--)
		*temp++ = val;
}

void memset32(uint32 *dst, uint32 val, size_t len)
{
	ASSERT(dst);

	uint32 *temp = (uint32 *)dst;
	do
	{
		*temp++ = val;
		len -= sizeof(uint32);
	} while(len);
}

int memcmp(uint8* one, uint8* two, size_t len){
	
	int i=0;
	while(i < len){
		
		if(one[i] != two[i])
			return 1;

		i++;
	}

	return 0;
}

int strcmp(char *str1, char *str2){

	int i = 0;

	if(ARRAYSIZE(str1) != ARRAYSIZE(str2))
		return 1;

	while(str1[i] != '\0' && str2[i] != '\0'){

		if(str1[i] != str2[i]){
			return 1;
		}
		i++;
	}
  	
	return 0;
}

void strcpy(char *dest, const char *src){

	do{
		*dest++ = *src++;
	}while (*src != 0);
}

char *strcat(char *dest, const char *src){

    while (*dest != 0){
        *dest = *dest++;
    }

    do{
        *dest++ = *src++;
    }while (*src != 0);

    return dest;
}


void zero(void* addr, size_t size)
{
	ASSERT( addr );

	if(size % sizeof(uint32))
	{
		memset(addr, 0, size);
	}
	else
	{
		memset32(addr, 0, size);
	}
	
}

