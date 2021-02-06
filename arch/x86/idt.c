#pragma once

#include "types.h"
#include "idt.h"
#include "../../drivers/screen.h"
#include "../../kernel/util.h"

void set_idt_descriptor()
{
    idt_descriptor.limit = sizeof(idt_gate_t) * IDT_ENTRIES - 1;
    idt_descriptor.base = (uint32_t)&idt_gates;

    // mmzero(&idt_gates, sizeof(idt_gate_t) * NR_INTERRUPTS);

    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_descriptor));
}

void _set_idt_gate(uint32_t num, uint8_t type, uint8_t dpl, uint32_t handler, uint8_t selector)
{
    idt_gate_t data;
    memzero(&data, sizeof(data));

    data.callback_low_byte = byte_low_16(handler);
    data.seg_selector = selector;
    data.flags.dpl = dpl;
    data.flags.type = type;
    data.flags.p = 1;
    data.callback_high_byte = byte_high_16(handler);

    memcopy(&data, &idt_gates[num], sizeof(data));
}

void set_trap_gate(uint32_t num, uint32_t handler)
{
    _set_idt_gate(num, GATE_TYPE_TRAP, DPL0, handler, KERNEL_CS);
}

void set_interrupt_gate(uint32_t num, uint32_t handler)
{
    _set_idt_gate(num, GATE_TYPE_INTERRUPT, DPL0, handler, KERNEL_CS);
}

void set_system_trap_gate(uint32_t num, uint32_t handler)
{
    _set_idt_gate(num, GATE_TYPE_TRAP, DPL3, handler, KERNEL_CS);
}

void set_system_interrupt_gate(uint32_t num, uint32_t handler)
{
    _set_idt_gate(num, GATE_TYPE_INTERRUPT, DPL3, handler, KERNEL_CS);
}

void init_traps()
{
    set_interrupt_gate(0, (uint32_t)isr_divide_by_zero);
    set_interrupt_gate(1, (uint32_t)isr_debug);
    set_interrupt_gate(2, (uint32_t)isr_nmi);
    set_system_interrupt_gate(3, (uint32_t)isr_breakpoint);
    set_system_interrupt_gate(4, (uint32_t)isr_overflow);
    set_interrupt_gate(5, (uint32_t)isr_boundrange);
    set_interrupt_gate(6, (uint32_t)isr_invalid_opcode);
    set_interrupt_gate(7, (uint32_t)isr_device_not_available);
    set_interrupt_gate(8, (uint32_t)isr_double_fault);
    set_interrupt_gate(9, (uint32_t)isr_segment_overrun);
    set_interrupt_gate(10, (uint32_t)isr_invalid_tss);
    set_interrupt_gate(11, (uint32_t)isr_segment_not_present);
    set_interrupt_gate(12, (uint32_t)isr_stack_segment_fault);
    set_interrupt_gate(13, (uint32_t)isr_general_protection);
    set_interrupt_gate(14, (uint32_t)isr_page_fault);
    set_interrupt_gate(15, (uint32_t)isr_reserved_15);
    set_interrupt_gate(16, (uint32_t)isr_fpe);
    set_interrupt_gate(17, (uint32_t)isr_alignment_check);
    set_interrupt_gate(18, (uint32_t)isr_machine_check);
    set_interrupt_gate(19, (uint32_t)isr_simd_fpe);
    set_interrupt_gate(20, (uint32_t)isr_virtualization_error);
    set_system_interrupt_gate(21, (uint32_t)isr_syscall);

    set_idt_descriptor();
}

char *ISR_EXCEPTION_MESSAGE[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
    ""
};

void isr_handler(registers_t regs)
{
    kprint("Received interrupt: ");
    char s[3];
    int_to_ascii(regs.int_number, s);
    kprint(s);
    kprint("\n");
    kprint(ISR_EXCEPTION_MESSAGE[regs.int_number]);
    kprint("\n");
}