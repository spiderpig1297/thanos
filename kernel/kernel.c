#include "../drivers/screen.h"
#include "util.h"
#include "../arch/x86/isr.h"

void main() {
    kclear_screen();

    kprint("Kernel started!\n");

    isr_install();

    __asm__ __volatile__("int $2");
    __asm__ __volatile__("int $3");

    int i = 5;
    int b = i / 0;
}