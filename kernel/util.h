#pragma once

#include "types.h"

void memcopy(const uint8_t* source, uint8_t* dest, uint32_t nbytes);
void memset(uint8_t* source, uint8_t value, uint32_t length);
void memzero(uint8_t* source, uint32_t length);

void int_to_ascii(uint32_t value, uint8_t* result);