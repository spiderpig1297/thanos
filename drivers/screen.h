#pragma once

#include "../kernel/arch/x86/common.h"

void kprint_at(const uint8_t* message, int32_t col, int32_t row);
void kprint(const uint8_t* message);
void kprint_err(const uint8_t* message);
void kclear_screen();