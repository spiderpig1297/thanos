#pragma once

#include "idt.h"
#include "isr.h"
#include "../../drivers/screen.h"
#include "../../kernel/util.h"

void set_idt_descriptor()
{
    idt_descriptor.limit = sizeof(idt_gate_t) * IDT_ENTRIES - 1;
    idt_descriptor.base = (uint32_t)&idt_gates;

    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_descriptor));
}

void _set_idt_gate(uint32_t num, uint8_t type, uint8_t dpl, uint32_t handler, uint8_t selector)
{
    idt_gate_t data;
    memzero(&data, sizeof(data));

    data.callback_low_byte = dword_low_16(handler);
    data.seg_selector = selector;
    data.flags.dpl = dpl;
    data.flags.type = type;
    data.flags.p = 1;
    data.callback_high_byte = dword_high_16(handler);

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

void init_idt()
{    
    set_idt_descriptor();

    /* CPU interrupts */
    set_interrupt_gate(X86_TRAP_DE,         (uint32_t)isr_divide_by_zero);
    set_interrupt_gate(X86_TRAP_DB,         (uint32_t)isr_debug);
    set_interrupt_gate(X86_TRAP_NMI,        (uint32_t)isr_nmi);
    set_system_interrupt_gate(X86_TRAP_BP,  (uint32_t)isr_breakpoint);
    set_system_interrupt_gate(X86_TRAP_OF,  (uint32_t)isr_overflow);
    set_interrupt_gate(X86_TRAP_BR,         (uint32_t)isr_boundrange);
    set_interrupt_gate(X86_TRAP_UD,         (uint32_t)isr_invalid_opcode);
    set_interrupt_gate(X86_TRAP_NM,         (uint32_t)isr_device_not_available);
    set_interrupt_gate(X86_TRAP_DF,         (uint32_t)isr_double_fault);
    set_interrupt_gate(X86_TRAP_OLD_MF,     (uint32_t)isr_segment_overrun);
    set_interrupt_gate(X86_TRAP_TS,         (uint32_t)isr_invalid_tss);
    set_interrupt_gate(X86_TRAP_NP,         (uint32_t)isr_segment_not_present);
    set_interrupt_gate(X86_TRAP_SS,         (uint32_t)isr_stack_segment_fault);
    set_interrupt_gate(X86_TRAP_GP,         (uint32_t)isr_general_protection);
    set_interrupt_gate(X86_TRAP_PF,         (uint32_t)isr_page_fault);
    set_interrupt_gate(X86_TRAP_INTEL_RES,  (uint32_t)isr_reserved_15);
    set_interrupt_gate(X86_TRAP_MF,         (uint32_t)isr_fpe);
    set_interrupt_gate(X86_TRAP_AC,         (uint32_t)isr_alignment_check);
    set_interrupt_gate(X86_TRAP_MC,         (uint32_t)isr_machine_check);
    set_interrupt_gate(X86_TRAP_XF,         (uint32_t)isr_simd_fpe);

    set_system_interrupt_gate(SYSCALL_VECTOR, (uint32_t)isr_syscall);

    remap_pic_irq();

    /* IRQs */
    set_interrupt_gate(X86_IRQ0,    (uint32_t)irq0);
    set_interrupt_gate(X86_IRQ1,    (uint32_t)irq1);
    set_interrupt_gate(X86_IRQ2,    (uint32_t)irq2);
    set_interrupt_gate(X86_IRQ3,    (uint32_t)irq3);
    set_interrupt_gate(X86_IRQ4,    (uint32_t)irq4);
    set_interrupt_gate(X86_IRQ5,    (uint32_t)irq5);
    set_interrupt_gate(X86_IRQ6,    (uint32_t)irq6);
    set_interrupt_gate(X86_IRQ7,    (uint32_t)irq7);
    set_interrupt_gate(X86_IRQ8,    (uint32_t)irq8);
    set_interrupt_gate(X86_IRQ9,    (uint32_t)irq9);
    set_interrupt_gate(X86_IRQ10,   (uint32_t)irq10);
    set_interrupt_gate(X86_IRQ11,   (uint32_t)irq11);
    set_interrupt_gate(X86_IRQ12,   (uint32_t)irq12);
    set_interrupt_gate(X86_IRQ13,   (uint32_t)irq13);
    set_interrupt_gate(X86_IRQ14,   (uint32_t)irq14);
    set_interrupt_gate(X86_IRQ15,   (uint32_t)irq15);
}