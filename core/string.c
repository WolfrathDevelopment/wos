/*
 * console.c
 Joel Wolfrath, 2013
 *
 * Impelentation of basic string functions
 */

#include "core.h"

/* Copy length bytes from src to dest */

void memcpy(w_uint8 *dest, const w_uint8 *src, w_uint32 length){

    	const w_uint8 *sp = (const w_uint8 *)src;
    	w_uint8 *dp = (w_uint8 *)dest;

    	for(; length != 0; length--)
			*dp++ = *sp++;
}

/* Write many copies of val to dest */

void memset(w_uint8 *dest, w_uint8 val, w_uint32 many){

	w_uint8 *temp = (w_uint8 *)dest;
	for ( ; many != 0; many--)
		*temp++ = val;
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
