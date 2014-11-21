/*
 * types.h
 * Joel Wolfrath, 2013
 *
 * Some basic types and definitions used in most
 * OS files.
 */

#ifndef __OS_TYPES_H
#define __OS_TYPES_H


/* Null is a useful definition to have */

#define NULL ((void*)0)


/* Macro to calculate the number of elements in an array */

#define ARRAYSIZE(x) sizeof(x)/sizeof(x[0])


/* Shortened unsigned types */

//typedef unsigned char uchar;
//typedef unsigned short ushort;
//typedef unsigned int uint;

typedef char w_int8;
typedef unsigned char w_uint8;
typedef short w_int16;
typedef unsigned short w_uint16;
typedef int w_int32;
typedef unsigned int w_uint32;
typedef long long w_int64;
typedef unsigned long long w_uint64;

typedef void* w_ptr;


/*
 * WARNING: ulong is defined as long long
 * These should always be 64 bits
 */

//typedef unsigned long long ulong;


/* A page directory entry */

typedef w_uint32 w_pde;


/* A page table entry */

typedef w_uint32 w_pte;


/* Helpful branch predictors */

#define likely(x)		(__builtin_expect(!!(x),1))
#define unlikely(x)		(__builtin_expect(!!(x),0))

#endif /* __OS_TYPES_H */

