#include "../drivers/screen.h"
#include "util.h"
#include "../arch/x86/idt.h"

void main() {
    kclear_screen();

    kprint("Kernel started!\n");

    init_idt();

    __asm__ __volatile__("int $2");
}