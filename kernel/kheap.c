#include "kheap.h"

#include "../arch/x86/paging.h"  // TODO: what if there will be another arch?

#define ALIGN       (1)
#define NOALIGN     (0)

uint32_t placement_address = 0;

uint32_t align_address(uint32_t address, uint32_t boundary)
{
    return (address & (boundary - 1)) ? (address & (~(boundary - 1)) + boundary): address; 
}

uint32_t kmalloc(uint32_t len)
{
    return _kmalloc(len, NOALIGN, NULL);
}

uint32_t kmalloca(uint32_t len)
{
    return _kmalloc(len, ALIGN, NULL);
}

uint32_t kmallocp(uint32_t len, uint32_t *physical_address)
{
    return _kmalloc(len, NOALIGN, physical_address);
}

uint32_t kmallocpa(uint32_t len, uint32_t *physical_address)
{
    return _kmalloc(len, ALIGN, physical_address);
}

uint32_t _kmalloc(uint32_t len, uint8_t align, uint32_t *physical_addr)
{
    if (1 == align && !IS_ADDRESS_ALIGNED(placement_address, PAGESIZE)) {
        placement_address = align_address(placement_address, PAGESIZE);
    }

    if (physical_addr) {
        *physical_addr = placement_address;
    }

    uint32_t t = placement_address;
    placement_address += len;
    return t;
}