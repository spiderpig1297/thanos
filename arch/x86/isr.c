#pragma once

#include "types.h"
#include "isr.h"
#include "idt.h"
#include "../../drivers/screen.h"
#include "../../kernel/util.h"

void isr_install()
{
    init_idt_entry(0, (uint32_t)isr0);
    init_idt_entry(1, (uint32_t)isr1);
    init_idt_entry(2, (uint32_t)isr2);
    init_idt_entry(3, (uint32_t)isr3);
    init_idt_entry(4, (uint32_t)isr4);
    init_idt_entry(5, (uint32_t)isr5);
    init_idt_entry(6, (uint32_t)isr6);
    init_idt_entry(7, (uint32_t)isr7);
    init_idt_entry(8, (uint32_t)isr8);
    init_idt_entry(9, (uint32_t)isr9);
    init_idt_entry(10, (uint32_t)isr10);
    init_idt_entry(11, (uint32_t)isr11);
    init_idt_entry(12, (uint32_t)isr12);
    init_idt_entry(13, (uint32_t)isr13);
    init_idt_entry(14, (uint32_t)isr14);
    init_idt_entry(15, (uint32_t)isr15);
    init_idt_entry(16, (uint32_t)isr16);
    init_idt_entry(17, (uint32_t)isr17);
    init_idt_entry(18, (uint32_t)isr18);
    init_idt_entry(19, (uint32_t)isr19);
    init_idt_entry(20, (uint32_t)isr20);
    init_idt_entry(21, (uint32_t)isr21);
    init_idt_entry(22, (uint32_t)isr22);
    init_idt_entry(23, (uint32_t)isr23);
    init_idt_entry(24, (uint32_t)isr24);
    init_idt_entry(25, (uint32_t)isr25);
    init_idt_entry(26, (uint32_t)isr26);
    init_idt_entry(27, (uint32_t)isr27);
    init_idt_entry(28, (uint32_t)isr28);
    init_idt_entry(29, (uint32_t)isr29);
    init_idt_entry(30, (uint32_t)isr30);
    init_idt_entry(31, (uint32_t)isr31);

    init_idt_descriptor();
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