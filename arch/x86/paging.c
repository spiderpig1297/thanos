#include "paging.h"

#include "../../kernel/kheap.h"
#include "../../kernel/bitmap.h"
#include "../../drivers/screen.h"
#include "isr.h"

/**
 * Index: the index in the array of bitmap_t.
 * Offset: the offset of the bitmap_t's bit matching the frame number.
 */
#define FRAME_INDEX_IN_BITMAP(frame_num)    (frame_num / 32)
#define OFFSET_IN_FRAME(frame_num)   (frame_num % 32)

bitmap_t* frames;   // bitmap_t is used for efficiency.
uint32_t frames_number;

page_directory_t* kernel_pgd;
page_directory_t* current_pgd;

extern uint32_t placement_address;

typedef struct {
    uint32_t frame_number;
    uint32_t frame_index_in_bitmap;
    uint32_t offset_in_frame;
} frame_info_t;

typedef struct {
    uint8_t present     : 1;
    uint8_t readonly    : 1;
    uint8_t usermode    : 1;
    uint8_t reserved    : 1;
    uint8_t id          : 1;
} page_fault_error_code_t;

static uint32_t find_first_available_frame()
{
    int frame_index, offset_in_frame = 0;
    for (frame_index = 0; frame_index < frames_number; ++frame_index) {
        if (is_bitmap_full(frames[frame_index])) {
            // Current bitmap is full, keep going.
            continue;
        }

        for (offset_in_frame = 0; offset_in_frame < sizeof(bitmap_t); ++offset_in_frame) {
            if (!bitmap_test(frames[frame_index], offset_in_frame)) {
                return frame_index * sizeof(bitmap_t) + offset_in_frame;
            }    
        }
    }

    return -ENOTFOUND;
}

static void _enable_paging()
{
    uint32_t cr0 = get_cr0();
    set_cr0(cr0 | 0x80000000);
}

static void _fill_frame_info(uint32_t frame_address, frame_info_t* frame_info)
{
    uint32_t frame_number = frame_address / PAGESIZE;
    frame_info->frame_number = frame_number;
    frame_info->frame_index_in_bitmap = FRAME_INDEX_IN_BITMAP(frame_number);
    frame_info->offset_in_frame = OFFSET_IN_FRAME(frame_number);
}

static void _set_frame(uint32_t frame_address)
{
    frame_info_t frame_info  = { 0 };
    _fill_frame_info(frame_address, &frame_info);
    bitmap_set( &frames[frame_info.frame_index_in_bitmap], 
                frame_info.offset_in_frame);
}

static void _clear_frame(uint32_t frame_address)
{
    frame_info_t frame_info  = { 0 };
    _fill_frame_info(frame_address, &frame_info);
    bitmap_clear(&frames[frame_info.frame_index_in_bitmap], 
                 frame_info.offset_in_frame);
}

static uint32_t _test_frame(uint32_t frame_address)
{
    frame_info_t frame_info  = { 0 };
    _fill_frame_info(frame_address, &frame_info);
    return bitmap_test( &frames[frame_info.frame_index_in_bitmap], 
                        frame_info.offset_in_frame);
}


void alloc_frame(page_t* page, uint8_t kernel, uint8_t writable)
{
    if (NULL != page->frame_address) {
        // Frame is already allocated!
        return;
    }

    uint32_t index = find_first_available_frame();
    if (-ENOTFOUND == (int32_t)index) {
        // PANIC("Failed to allocate frame - not available frames.");
    }

    // Mark the frame as allocated.
    _set_frame(index);

    page->present = 1;
    page->usermode = kernel ? 0 : 1;
    page->writable = writable ? 1 : 0;
    page->frame_address = index;
}

void free_frame(page_t* page)
{
    uint32_t frame = page->frame_address;
    if (!frame) {
        // Nothing to free.
        return;
    }

    _clear_frame(frame);
}

void init_paging()
{
    frames_number = MEMORY_LIMIT / PAGESIZE;
    uint32_t frames_bitmap_size = FRAME_INDEX_IN_BITMAP(frames_number);
    frames = (uint32_t*)kmalloc(frames_bitmap_size);
    memzero(frames, frames_bitmap_size);

    kernel_pgd = (uint32_t*)kmalloca(sizeof(page_directory_t));
    memzero(kernel_pgd, sizeof(page_directory_t));
    current_pgd = kernel_pgd;

    // We need to identity map (phys addr = virt addr) from
    // 0x0 to the end of used memory, so we can access this
    // transparently, as if paging wasn't enabled.
    // NOTE that we use a while loop here deliberately.
    // inside the loop body we actually change placement_address
    // by calling kmalloc(). A while loop causes this to be
    // computed on-the-fly rather than once at the start.
    int i = 0;
    while (i < placement_address)
    {        
        kprint_dec((uint32_t)placement_address);
        // Kernel code is readable but not writeable from userspace.
        alloc_frame( get_page(i, 1, kernel_pgd), 0, 0);
        i += 0x1000;
    }

    register_interrupt_handler(X86_TRAP_PF, page_fault_handler);

    switch_page_directory(kernel_pgd);
}

void switch_page_directory(page_directory_t* new_pgd)
{
    current_pgd = new_pgd;
    set_cr3(&new_pgd->phys_tables);
    _enable_paging();
}

page_t* get_page(uint32_t address, uint8_t create, page_directory_t* pgdt)
{
    /**
     * Every address is composed of page number (higher 20 bits) and an offset
     * within that page (lower 12 bits). In order to find the page number, we
     * need to divide the address with PAGESIZE. 
     */
    address /= PAGESIZE;

    /**
     * Each page table is consisted from __NR_pages pages. In order to find the
     * right table, we need to further divide our address by __NR_pages.
     */
    uint32_t page_table_index = address / __NR_pages;

    if (pgdt->tables[page_table_index]) {
        /**
         * If the page table already exists, find the page and return it.
         */
        goto found_page;
    }

    /** 
     * If we have reached here, it means that the matching page table doesn't exist.
     * Create it if needed.
     */
    if (create) {
        uint32_t physical_addr;
        pgdt->tables[page_table_index] = (uint32_t*)kmallocpa(sizeof(page_table_t), &physical_addr);
        memzero(&pgdt->tables[page_table_index], sizeof(page_table_t));
        pgdt->phys_tables[page_table_index] = physical_addr | 0x7;  // PRESENT, RW, US.
        goto found_page;
    }

    return -ENOTFOUND;

found_page:
    return &pgdt->tables[page_table_index]->pages[address % __NR_pages];
}

void page_fault_handler(registers_t registers)
{
    page_fault_error_code_t page_fault_error = { 0 };
    
    uint32_t faulting_address = get_cr2();
    page_fault_error = *(page_fault_error_code_t*)&faulting_address;

    kprint("Page fault!\n");
    if (page_fault_error.present) kprint("present ");
    if (page_fault_error.readonly) kprint("readonly ");
    if (page_fault_error.usermode) kprint("usermode ");
    if (page_fault_error.reserved) kprint("reserved ");

    kprint("Panicking\n");
    for(;;);
}