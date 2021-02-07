#include "registers.h"

uint32_t get_cr0() 
{
    uint32_t cr0;                                             
    __asm__ __volatile__("mov %%cr0, %0" : "=r" (cr0));  
    return cr0;
}

void set_cr0(uint32_t val)
{
    __asm__ __volatile__("mov %0, %%cr0" :: "r" (val));
}

uint32_t get_cr2() 
{
    uint32_t cr2;                                             
    __asm__ __volatile__("mov %%cr2, %0" : "=r" (cr2));  
    return cr2;
}

void set_cr2(uint32_t val)
{
    __asm__ __volatile__("mov %0, %%cr2" :: "r" (val));
}

uint32_t get_cr3() 
{
    uint32_t cr3;                                             
    __asm__ __volatile__("mov %%cr3, %0" : "=r" (cr3));  
    return cr3;
}

void set_cr3(uint32_t val)
{
    __asm__ __volatile__("mov %0, %%cr3" :: "r" (val));
}