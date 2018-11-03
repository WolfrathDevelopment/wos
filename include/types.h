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


/* Shortened types */

typedef char                int8_t;
typedef unsigned char       uint8_t;
typedef short               int16_t;
typedef unsigned short      uint16_t;
typedef int                 int32_t;
typedef unsigned int        uint32_t;

typedef uint32_t            size_t;

// JSW TODO remove   
typedef char                int8;
typedef unsigned char       uint8;
typedef short               int16;
typedef unsigned short      uint16;
typedef int                 int32;
typedef unsigned int        uint32;
typedef long long           int64;
typedef unsigned long long  uint64;

/* Helpful branch predictors from Linux kernel */

#define likely(x)     (__builtin_expect(!!(x),1))
#define unlikely(x)   (__builtin_expect(!!(x),0))

/* Some return codes */

typedef enum {

    RC_SUCCESS         = 0x00,
    RC_INVALID_PARM    = 0x01,
    RC_ALLOC_FAILED    = 0x02,

} OsRc;

#endif /* __OS_TYPES_H */

