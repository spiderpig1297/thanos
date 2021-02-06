#pragma once

#include "types.h"
#include "registers.h"

#define __NR_INTERRUPTS (256)

#define enable_interrupts()   __asm__ __volatile__("sti")
#define disable_interrupts()   __asm__ __volatile__("cli")

/* PIC related consts */
#define MASTER_PIC_BASE_ADDRESS     (0x20)  /* Base address for master PIC */
#define SLAVE_PIC_BASE_ADDRESS      (0xA0)  /* Base address for slave PIC */
#define MASTER_PIC_COMMAND_REG      (MASTER_PIC_BASE_ADDRESS)         /* Master PIC command register */
#define MASTER_PIC_DATA_REG         (MASTER_PIC_BASE_ADDRESS + 1)     /* Master PIC data register */
#define SLAVE_PIC_COMMAND_REG       (SLAVE_PIC_BASE_ADDRESS)          /* Slave PIC command register */
#define SLAVE_PIC_DATA_REG          (SLAVE_PIC_BASE_ADDRESS + 1)      /* Slave PIC data register */

#define MASTER_PIC_NEW_MAPPING      (0x20)
#define SLAVE_PIC_NEW_MAPPING       (0x28)

/* PIC commands */
#define ICW_ENABLE_IRQ  (0x0)
#define ICW1_INIT       (0x10)  /* Initialize the PIC */
#define ICW1_ICW4       (0x01)  /* Prepare the PIC for upcoming commands */
#define EOI             (0x20)  /* End of interrupt */

enum {
    X86_IRQ0 = 32,
    X86_IRQ1,
    X86_IRQ2,
    X86_IRQ3,
    X86_IRQ4,
    X86_IRQ5,
    X86_IRQ6,
    X86_IRQ7,
    X86_IRQ8,
    X86_IRQ9,
    X86_IRQ10,
    X86_IRQ11,
    X86_IRQ12,
    X86_IRQ13,
    X86_IRQ14,
    X86_IRQ15 = 47,
};

typedef void (*interrupt_handler_t)(registers_t);

void register_interrupt_handler(uint8_t n, interrupt_handler_t handler);
void unregister_interrupt_handler(uint8_t n);

void irq_handler(registers_t regs);
void isr_handler(registers_t regs);

/**
 * Remaps PIC IRQs from 0x0...0x0F to 0x20...0x2F so it won't collide with
 * the CPU's default interrupts (0-32).
 */
void remap_pic_irq();

/* ISR defintions */
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
extern void isr_syscall();

/* IRQ definitions */
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();