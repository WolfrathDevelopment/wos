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

#define NULL    ((void*)0)
#define TRUE    (0x01)
#define FALSE   (0x00)


/* Macro to calculate the number of elements in an array */

#define ARRAYSIZE(x) sizeof(x)/sizeof(x[0])


/* Shortened unsigned types */

typedef char			int8;
typedef unsigned char		uint8;
typedef short			int16;
typedef unsigned short		uint16;
typedef int			int32;
typedef unsigned int		uint32;
typedef long long		int64;
typedef unsigned long long	uint64;

typedef uint32	size_t;

/* Helpful branch predictors */

#define likely(x)		(__builtin_expect(!!(x),1))
#define unlikely(x)		(__builtin_expect(!!(x),0))

#endif /* __OS_TYPES_H */

