#pragma once

#include "types.h"

typedef uint32_t bitmap_t;

void bitmap_set(bitmap_t* b, uint8_t i, uint8_t v)
{
    *b |= (0x1 << i);
}

uint8_t bitmap_test(const bitmap_t* b, uint8_t i)
{
    return ((*b >> i) & 0x1);
}

void bitmap_clear(bitmap_t* b, uint8_t i)
{
    *b &= ~(0x1 << i); 
}

