#include "ports.h"

uint8_t port_byte_in(uint16_t port)
{
    // A handy C wrapper function that reads a byte from the specified port.
    // "=a" ( result ): put AL register in variable RESULT when finished.
    // "d" ( port ): load EDX with port.
    uint8_t result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_byte_out(uint16_t port, uint8_t data)
{
    // "a": load EAX with data.
    // "d": EDX with port number.
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint16_t port_word_in(uint16_t port)
{
    uint16_t result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void port_word_out(uint16_t port, uint16_t data)
{
    // "a": load EAX with data.
    // "d": EDX with port number.
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}

