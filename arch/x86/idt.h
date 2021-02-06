#pragma once

#include "types.h"

#define KERNEL_CS (0x08)

#define NR_INTERRUPTS (256)

// Describes an interrupt handler.
typedef struct
{
    uint16_t callback_low_byte;     // Lower 16bit of the callback function address.
    uint16_t seg_selector;          // Kernel segment selector.
    uint8_t zero;                   // Always 0.

    // Additional flags:
    //  bits 0-4 - will always be 14.
    //  bits 5-6 - DPL.
    //  bits 7 - signifies if handler is present.
    uint8_t flags;      
                
    uint16_t callback_high_byte;    // Higher 16bit of the callback function address.
} __attribute__((packed)) idt_gate_t;

// Struct describes a pointer to the first interrupt handler in the system.
typedef struct 
{
    uint16_t limit;
    uint32_t base;      // Address of the first idt_entry_t struct.
} __attribute__((packed)) idt_descriptor_t;

idt_gate_t idt_gates[NR_INTERRUPTS];
idt_descriptor_t idt_descriptor;

void set_idt_descriptor();
void set_idt_gate(uint32_t num, uint32_t handler);