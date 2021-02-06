#pragma once

#include "types.h"

extern void isr_divide_by_zero();
extern void isr_debug();
extern void isr_nmi();
extern void isr_breakpoint();
extern void isr_overflow();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

typedef struct {
    uint32_t ds;                                        // Data segment selector.
    uint32_t eax, ebx, ecx, edx, edi, esi, ebp, esp;    // General purpose registers. Pushed by pusha.
    uint32_t int_number, err_code;                      // Interrupt number and error code.
    uint32_t eip, cs, eflags, useresp, ss;              // Pushed automatically by the CPU.
} registers_t;  

void isr_install();
void isr_handler(registers_t regs);