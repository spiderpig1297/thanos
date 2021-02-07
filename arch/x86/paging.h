#pragma once

#include "kernel-generic.h"
#include "registers.h"

#define __NR_pages (1024)

typedef struct {
    uint8_t present         : 1;
    uint8_t writable        : 1;
    uint8_t usermode        : 1;
    uint8_t accessed        : 1;
    uint8_t dirty           : 1;
    uint8_t unused          : 7;    // Amalgamation of unused and reserved bits.
    uint32_t frame_address  : 20;
} page_t;

typedef struct {
    page_t pages[__NR_pages];
} page_table_t;

typedef struct {
    /**
     * Array of pointers to all page tables.
     */
    page_table_t* tables[__NR_pages];

    /**
     * Physical addresses of all the page tables above, to load to CR3 register.  
     */
    uint32_t phys_tables[__NR_pages];   

    /**
     * The physical address of tablesPhysical. This comes into play
     * when we get our kernel heap allocated and the directory
     * may be in a different location in virtual memory.
     */
    uint32_t physicalAddr;
} page_directory_t;

void init_paging();

void switch_page_directory(page_directory_t* new_pgd);

/**
 * Retrieves a pointer to the required page.
 * Setting create to 1 will create the page if doesn't exist.
 */
page_t* get_page(uint32_t address, uint8_t create, page_directory_t* pgdt);

/**
 * Handler for page faults.
 */
void page_fault_handler(registers_t registers);