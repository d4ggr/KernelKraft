/*
 * types.h - Basic type definitions for the kernel
 *
 *Equivalent of stdint.h, stdbool.h, etc.
 * 
 */

#ifndef TYPES_H
#define TYPES_H

/* Fixed-width integer types */
typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;

typedef signed char         int8_t;
typedef signed short        int16_t;
typedef signed int          int32_t;
typedef signed long long    int64_t;

/* Pointer-sized integer */
typedef uint64_t            uintptr_t;
typedef int64_t             intptr_t;
typedef uint64_t            size_t;

/* Boolean */
typedef uint8_t             bool;
#define true  1
#define false 0

/* NULL pointer */
#define NULL ((void *)0)

/* Useful macros */
#define ARRAY_SIZE(x)   (sizeof(x) / sizeof((x)[0]))
#define ALIGN_UP(x, a)  (((x) + ((a) - 1)) & ~((a) - 1))
#define ALIGN_DOWN(x,a) ((x) & ~((a) - 1))

#endif /* TYPES_H */
