#include "../drivers/screen.h"
#include "util.h"
#include "../arch/x86/idt.h"
#include "../arch/x86/paging.h"
#include "../arch/x86/isr.h"
#include "../arch/x86/timer.h"

int main() 
{
    kclear_screen();

    kprint("Kernel started!\n");

    enable_interrupts();
    
    /* No need to init the GDT as the bootsector already did */

    init_idt();

    init_paging();

    uint32_t* ptr = (uint32_t*)0xA0000000;
    uint32_t do_page_fault = *ptr;

    return 0;
}