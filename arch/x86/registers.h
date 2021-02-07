#pragma once

#include "types.h"

typedef struct {
    uint32_t ds;                                        // Data segment selector.
    uint32_t eax, ebx, ecx, edx, edi, esi, ebp, esp;    // General purpose registers. Pushed by pusha.
    uint32_t int_number, err_code;                      // Interrupt number and error code.
    uint32_t eip, cs, eflags, useresp, ss;              // Pushed automatically by the CPU.
} registers_t;  

#define _DEFINE_CRX_GETTER(n)                                   \
    uint32_t inline get_cr##n() {                               \
        uint32_t cr##n;                                         \
        __asm__ __volatile__("mov %%cr##n, %0" : "=r" (cr##n)); \
        return cr##n;                                           \
    }                                                           \

#define _DEFINE_CRX_SETTER(n)                                   \
    void inline set_cr##n(uint32_t val)                         \
    {                                                           \
        __asm__ __volatile__("mov %0, %%cr##n" :: "r" (val));   \
    }                                                           \

// Define getter and setter for CR0.
_DEFINE_CRX_GETTER(0);
_DEFINE_CRX_GETTER(0);

// Define getter and setter for CR2.
_DEFINE_CRX_GETTER(2);
_DEFINE_CRX_GETTER(2);

// Define getter and setter for CR3.
_DEFINE_CRX_GETTER(3);
_DEFINE_CRX_SETTER(3);