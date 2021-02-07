#pragma once

#include "types.h"

#define IDT_ENTRIES (256)
#define SYSCALL_VECTOR (0x88)

#define KERNEL_CS (0x08)

/* Interrupt gate types */
#define GATE_TYPE_INTERRUPT (14)
#define GATE_TYPE_TRAP (15)

/* Privilege levels */
#define DPL0 (0)
#define DPL3 (3)

typedef struct {
	uint16_t    ist	 : 3,
			    zero : 5,
			    type : 5,
			    dpl	 : 2,
			    p	 : 1;
} __attribute__((packed)) idt_bits;

typedef struct
{
    uint16_t callback_low_byte;     // Lower 16bit of the callback function address.
    uint16_t seg_selector;          // Kernel segment selector.
    idt_bits flags;                 
    uint16_t callback_high_byte;    // Higher 16bit of the callback function address.
} __attribute__((packed)) idt_gate_t;

typedef struct 
{
    uint16_t limit;
    uint32_t base;      // Address of the first idt_entry_t struct.
} __attribute__((packed)) idt_descriptor_t;

idt_gate_t idt_gates[IDT_ENTRIES];
idt_descriptor_t idt_descriptor;

void set_idt_descriptor();

/* Interrupt gates initializers */
void init_traps();
void set_interrupt_gate(uint32_t num, uint32_t handler);
void set_trap_gate(uint32_t num, uint32_t handler);
void set_system_trap_gate(uint32_t num, uint32_t handler);
void set_system_interrupt_gate(uint32_t num, uint32_t handler);
void _set_idt_gate(uint32_t num, uint8_t type, uint8_t dpl, uint32_t handler, uint8_t selector);