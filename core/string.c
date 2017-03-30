/*
 * console.c
 Joel Wolfrath, 2013
 *
 * Impelentation of basic string functions
 */

#include "core.h"

/* Copy length bytes from src to dest */

void memcpy(uint8 *dest, const uint8 *src, size_t len){

	const uint8 *sp = (const uint8 *)src;
	uint8 *dp = (uint8 *)dest;

	while(len--)
		*dp++ = *sp++;
}

/* Write many copies of val to dest */

void memset(uint8 *dest, uint8 val, size_t many){

	uint8 *temp = (uint8 *)dest;
	while(many--)
		*temp++ = val;
}

void memset32(uint32 *dest, uint32 val, size_t cnt){

	uint32 *temp = (uint32 *)dest;
	while(cnt--)
		*temp++ = val;
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

/* Compare two strings. Returns 0 if equal, else 1 */

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

/* Copy the NULL-terminated string src to dest */

void strcpy(char *dest, const char *src){

	do{
		*dest++ = *src++;
	}while (*src != 0);
}

/* Concatenate src to the end of dest */

char *strcat(char *dest, const char *src){

    while (*dest != 0){
        *dest = *dest++;
    }

    do{
        *dest++ = *src++;
    }while (*src != 0);

    return dest;
}
