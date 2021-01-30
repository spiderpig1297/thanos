#include "idt.h"

void init_idt_descriptor()
{
    idt_descriptor.limit = sizeof(idt_entry_t) * IDT_ENTRIES - 1;
    idt_descriptor.base = (uint32_t)&idt_entries;

    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_descriptor));
}

void init_idt_entry(uint32_t num, uint32_t handler)
{
    idt_entries[num].callback_low_byte = byte_low_16(handler);
    idt_entries[num].seg_selector = KERNEL_CS;
    idt_entries[num].zero = 0;
    idt_entries[num].flags = 0x8e;
    idt_entries[num].callback_high_byte = byte_high_16(handler);
}