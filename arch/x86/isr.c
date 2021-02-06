#include "isr.h"

#include "../../drivers/screen.h"

interrupt_handler_t interrupt_handlers[__NR_INTERRUPTS];

void register_interrupt_handler(uint8_t n, interrupt_handler_t handler) 
{
    if (__NR_INTERRUPTS > n) {
        // TODO: implement!
    }

    interrupt_handlers[n] = handler;
}

void unregister_interrupt_handler(uint8_t n) 
{
    if (__NR_INTERRUPTS > n) {
        // TODO: implement!
    }

    interrupt_handlers[n] = 0;
}

void remap_pic_irq()
{   
    kprint("Remapping PIC IRQ lines\n");

    uint8_t master_pic_original_data = 0;
    uint8_t slave_pic_original_data = 0;

    master_pic_original_data = inb(MASTER_PIC_DATA_REG);
    slave_pic_original_data = inb(SLAVE_PIC_DATA_REG);

    // Initialize the Master and Slave PICs and prepare them for upcoming commands.
    outb(MASTER_PIC_BASE_ADDRESS, ICW1_INIT | ICW1_ICW4);
    outb(SLAVE_PIC_BASE_ADDRESS, ICW1_INIT | ICW1_ICW4);

    outb(MASTER_PIC_DATA_REG, MASTER_PIC_NEW_MAPPING);
    outb(SLAVE_PIC_DATA_REG, SLAVE_PIC_NEW_MAPPING);

    outb(MASTER_PIC_DATA_REG, 0x04);  /* Tell Master PIC that there is a slave at IRQ2 */
    outb(SLAVE_PIC_DATA_REG, 0x02);

    outb(MASTER_PIC_DATA_REG, ICW1_ICW4);
    outb(SLAVE_PIC_DATA_REG, ICW1_ICW4);

    outb(MASTER_PIC_DATA_REG, ICW_ENABLE_IRQ);    /* Enable all IRQ on Master PIC. */
    outb(SLAVE_PIC_DATA_REG, ICW_ENABLE_IRQ);    /* Enable all IRQ on Slave PIC. */

    // Restore saved masks.
    outb(MASTER_PIC_DATA_REG, master_pic_original_data);
    outb(SLAVE_PIC_DATA_REG, slave_pic_original_data);
}

void irq_handler(registers_t registers)
{
    // If the interrupt involved the slave, send an EOI to the PIC.
    if (SLAVE_PIC_NEW_MAPPING <= registers.int_number) {
        outb(SLAVE_PIC_BASE_ADDRESS, EOI);
    }

    // Either case - send EOI to the master too.
    outb(MASTER_PIC_BASE_ADDRESS, EOI);

    if (0 != interrupt_handlers[registers.int_number]) {
        interrupt_handler_t handler = interrupt_handlers[registers.int_number];
        handler(registers);
    }
}

void isr_handler(registers_t registers)
{
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
        ""
    };

    kprint("Received interrupt: ");
    char s[3];
    int_to_ascii(registers.int_number, s);
    kprint(s);
    kprint("\n");
    kprint(ISR_EXCEPTION_MESSAGE[registers.int_number]);
    kprint("\n");
}