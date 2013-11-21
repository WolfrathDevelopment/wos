/*
 * types.h
 * Wolfrath/Kriewall, 2013
 * 
 * Some basic types and definitions
 */

#define NULL ((void*)0)
#define ARRAYSIZE(x) sizeof(x)/sizeof(x[0])

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long long ulong;

/* A page directory entry */
typedef uint w_pde;

/* A page table entry */
typedef uint w_pte;
