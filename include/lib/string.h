/*
 * string.h
 * Joel Wolfrath, 2015
 * 
 * Basic standard library string functions
 */

#ifndef _OS_STRING_H
#define _OS_STRING_H

#include <types.h>

void memcpy(void* dst, const void* src, size_t len);

void memset(uint8_t* dst, uint8_t val, size_t len);

void memset32(uint32_t* dst, uint32_t val, size_t len);

int32_t memcmp(uint8_t* m1, uint8_t* m2, size_t len);

void zero(void* dst, size_t len);

#endif /* _OS_STRING_H */
