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

enum {
    X86_TRAP_DE = 0,	/*  0, Divide-by-zero */
	X86_TRAP_DB,		/*  1, Debug */
	X86_TRAP_NMI,		/*  2, Non-maskable Interrupt */
	X86_TRAP_BP,		/*  3, Breakpoint */
	X86_TRAP_OF,		/*  4, Overflow */
	X86_TRAP_BR,		/*  5, Bound Range Exceeded */
	X86_TRAP_UD,		/*  6, Invalid Opcode */
	X86_TRAP_NM,		/*  7, Device Not Available */
	X86_TRAP_DF,		/*  8, Double Fault */
	X86_TRAP_OLD_MF,	/*  9, Coprocessor Segment Overrun */
	X86_TRAP_TS,		/* 10, Invalid TSS */
	X86_TRAP_NP,		/* 11, Segment Not Present */
	X86_TRAP_SS,		/* 12, Stack Segment Fault */
	X86_TRAP_GP,		/* 13, General Protection Fault */
	X86_TRAP_PF,		/* 14, Page Fault */
	X86_TRAP_INTEL_RES,	/* 15, Intel-reserved */
	X86_TRAP_MF,		/* 16, x87 Floating-Point Exception */
	X86_TRAP_AC,		/* 17, Alignment Check */
	X86_TRAP_MC,		/* 18, Machine Check */
	X86_TRAP_XF,		/* 19, SIMD Floating-Point Exception */
	X86_TRAP_IRET = 32,
};

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