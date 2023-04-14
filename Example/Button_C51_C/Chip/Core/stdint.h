/**
 * @file      stdint.h
 * @author    XiaoNing
 * @post      Embedded Software Engineer
 * @brief     stdint.h redefine
 * @version   1.0.0
 * @date      2023-03-14
 * @copyright Copyright (c) 2023
 */
#ifndef __STDINT_H
#define __STDINT_H

typedef signed char         int8_t;
typedef signed short int    int16_t;
typedef signed long int     int32_t;

typedef unsigned char       uint8_t;
typedef unsigned short int  uint16_t;
typedef unsigned long int   uint32_t;

#define INT8_MIN            -128
#define INT16_MIN           -32768
#define INT32_MIN           -2147483648

#define INT8_MAX            127
#define INT16_MAX           32767
#define INT32_MAX           2147483647

#define UINT8_MAX           255u
#define UINT16_MAX          65535u
#define UINT32_MAX          4294967295u

#endif /* __STDINT_H */
