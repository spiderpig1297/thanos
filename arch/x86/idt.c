#include "idt.h"

void set_idt_descriptor()
{
    idt_descriptor.limit = sizeof(idt_gate_t) * NR_INTERRUPTS - 1;
    idt_descriptor.base = (uint32_t)&idt_gates;

    // mmzero(&idt_gates, sizeof(idt_gate_t) * NR_INTERRUPTS);

    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_descriptor));
}

void set_idt_gate(uint32_t num, uint8_t type, uint8_t dpl, uint32_t handler)
{
    idt_gates[num].callback_low_byte = byte_low_16(handler);
    idt_gates[num].seg_selector = KERNEL_CS;
    idt_gates[num].zero = 0;
    idt_gates[num].flags = 0x8F;
    idt_gates[num].callback_high_byte = byte_high_16(handler);
}