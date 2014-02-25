/*
 * types.h
 * Wolfrath/Kriewall, 2013
 *
 * Some basic types and definitions used in most
 * OS files.
 */


/* Null is a useful definition to have */

#define NULL ((void*)0)


/* Macro to calculate the number of elements in an array */

#define ARRAYSIZE(x) sizeof(x)/sizeof(x[0])


/* Shortened unsigned types */

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;


/*
 * WARNING: ulong is defined as long long
 * These should always be 64 bits
 */

typedef unsigned long long ulong;


/* A page directory entry */

typedef uint w_pde;


/* A page table entry */

typedef uint w_pte;
