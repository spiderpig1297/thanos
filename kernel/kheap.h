#pragma once

#include "types.h"

#define PAGESIZE (4096)

#define IS_ADDRESS_ALIGNED(address, boundary) !(address & (boundary - 1))

uint32_t align_address(uint32_t address, uint32_t boundary);

uint32_t kmalloc(uint32_t len);
uint32_t kmalloca(uint32_t len);
uint32_t kmallocp(uint32_t len, uint32_t *physical_address);
uint32_t kmallocpa(uint32_t len, uint32_t *physical_address);
uint32_t _kmalloc(uint32_t len, uint8_t align, uint32_t *physical_addr);