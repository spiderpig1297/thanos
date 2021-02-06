#pragma once

// 32-bit x86 typedefs.
typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;

#define word_low_8(word)    ((uint8_t)(word & 0xFF))
#define word_high_8(word)   ((uint8_t)((word >> 8) & 0xFF))
#define dword_low_16(dword)   ((uint16_t)(dword & 0xFFFF))
#define dword_high_16(dword)  ((uint16_t)((dword >> 16) & 0xFFFF))

