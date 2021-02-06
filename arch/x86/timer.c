#include "timer.h"
#include "isr.h"
#include "../../drivers/screen.h"

#define PIT_DEFAULT_FREQ    (1193180)  // in HZ

#define PIT_COMMAND_REG     (0x43)
#define PIT_DATA_REG        (0x40)
#define PIT_SET_REPEATING   (0x36)

uint32_t tick = 0;

static void timer_callback(registers_t registers)
{
    ++tick;
    kprint("Tick: ");
    kprint("\n");
}

void init_timer(uint32_t frequency)
{
    register_interrupt_handler(X86_IRQ0, &timer_callback);

    // Note that the divisor must be small enough to fit in 16-bit integer.
    uint32_t divisor = PIT_DEFAULT_FREQ / frequency;

    // Set PIT to repeating mode, so when the timer reaches zero it will automatically update.
    outb(PIT_COMMAND_REG, PIT_SET_REPEATING);

    // Divisor has to be sent byte-wise
    uint8_t divisor_lower_byte = word_low_8(divisor);
    outb(PIT_DATA_REG, divisor_lower_byte);

    uint8_t divisor_higher_byte = word_high_8(divisor);
    outb(PIT_DATA_REG, divisor_higher_byte);
}
