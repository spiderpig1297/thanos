#pragma once

#include "../arch/x86/types.h"

void mmcopy(const uint8_t* source, uint8_t* dest, uint32_t nbytes);
void mmset(uint8_t* source, uint8_t value, uint32_t length);
void mmzero(uint8_t* source, uint32_t length);

void int_to_ascii(uint32_t value, uint8_t* result);