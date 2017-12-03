/*
 * string.h
 * Joel Wolfrath, 2015
 * 
 * Basic standard library string functions
 */

#ifndef _STRING_H
#define _STRING_H

#include <types.h>

/* Copies *len* bytes from *src* to *dst* */
void memcpy(uint8 * dst, const uint8 * src, size_t len);

/* Writes *len* copies of *val* to *dst* */ 
void memset(uint8 * dst, uint8 val, size_t len);

/* Writes *(len / 4)* copies of *val* to *dst* */ 
void memset32(uint32 * dst, uint32 val, size_t len);

int memcmp(uint8*, uint8*, size_t);
int32 strcmp(char *, char *);
void strcpy(char *, const char *);
char *strcat(char *, const char *);
void zero(void *, size_t);

#endif /* _STRING_H */
