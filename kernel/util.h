#pragma once

#include "../arch/x86/types.h"

void memory_copy(const uint8_t* source, uint8_t* dest, uint32_t nbytes);
void int_to_ascii(uint32_t value, uint8_t* result);
void memory_set(uint8_t* source, uint8_t value, uint32_t length);