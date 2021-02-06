#include "../drivers/screen.h"
#include "util.h"
#include "../arch/x86/idt.h"
#include "../arch/x86/isr.h"
#include "../arch/x86/timer.h"

void main() {
    kclear_screen();

    kprint("Kernel started!\n");

    enable_interrupts();
    
    init_idt();
    init_timer(50);

    __asm__ __volatile__("int $32");
}