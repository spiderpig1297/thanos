#pragma once

// 32-bit x86 typedefs.
typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;

#define byte_low_16(byte) ((uint16_t)(byte & 0xFFFF))
#define byte_high_16(byte) ((uint16_t)((byte >> 16) & 0xFFFF))