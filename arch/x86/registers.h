#pragma once

#include "types.h"

typedef struct {
    uint32_t ds;                                        // Data segment selector.
    uint32_t eax, ebx, ecx, edx, edi, esi, ebp, esp;    // General purpose registers. Pushed by pusha.
    uint32_t int_number, err_code;                      // Interrupt number and error code.
    uint32_t eip, cs, eflags, useresp, ss;              // Pushed automatically by the CPU.
} registers_t;  
