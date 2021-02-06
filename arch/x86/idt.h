#pragma once

#include "types.h"

#define IDT_ENTRIES (256)

#define KERNEL_CS (0x08)

#define GATE_TYPE_INTERRUPT (14)
#define GATE_TYPE_TRAP (15)

#define DPL0 (0)
#define DPL3 (3)

typedef struct {
	uint16_t    ist	 : 3,
			    zero : 5,
			    type : 5,
			    dpl	 : 2,
			    p	 : 1;
} __attribute__((packed)) idt_bits;

// Describes an interrupt handler.
typedef struct
{
    uint16_t callback_low_byte;     // Lower 16bit of the callback function address.
    uint16_t seg_selector;          // Kernel segment selector.

    idt_bits flags;    
                
    uint16_t callback_high_byte;    // Higher 16bit of the callback function address.
} __attribute__((packed)) idt_gate_t;

// Struct describes a pointer to the first interrupt handler in the system.
typedef struct 
{
    uint16_t limit;
    uint32_t base;      // Address of the first idt_entry_t struct.
} __attribute__((packed)) idt_descriptor_t;


idt_gate_t idt_gates[IDT_ENTRIES];
idt_descriptor_t idt_descriptor;

extern void isr_divide_by_zero();
extern void isr_debug();
extern void isr_nmi();
extern void isr_breakpoint();
extern void isr_overflow();
extern void isr_boundrange();
extern void isr_invalid_opcode();
extern void isr_device_not_available();
extern void isr_double_fault();
extern void isr_segment_overrun();
extern void isr_invalid_tss();
extern void isr_segment_not_present();
extern void isr_stack_segment_fault();
extern void isr_general_protection();
extern void isr_page_fault();
extern void isr_reserved_15();
extern void isr_fpe();
extern void isr_alignment_check();
extern void isr_machine_check();
extern void isr_simd_fpe();
extern void isr_virtualization_error();
extern void isr_syscall();

typedef struct {
    uint32_t ds;                                        // Data segment selector.
    uint32_t eax, ebx, ecx, edx, edi, esi, ebp, esp;    // General purpose registers. Pushed by pusha.
    uint32_t int_number, err_code;                      // Interrupt number and error code.
    uint32_t eip, cs, eflags, useresp, ss;              // Pushed automatically by the CPU.
} registers_t;  

void isr_handler(registers_t regs);

void set_idt_descriptor();

void init_traps();
void set_interrupt_gate(uint32_t num, uint32_t handler);
void set_trap_gate(uint32_t num, uint32_t handler);
void set_system_trap_gate(uint32_t num, uint32_t handler);
void set_system_interrupt_gate(uint32_t num, uint32_t handler);
void _set_idt_gate(uint32_t num, uint8_t type, uint8_t dpl, uint32_t handler, uint8_t selector);
