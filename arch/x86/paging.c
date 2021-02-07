#include "paging.h"

#include "../../kernel/kheap.h"
#include "../../kernel/bitmap.h"

/**
 * Index: the index in the array of bitmap_t.
 * Offset: the offset of the bitmap_t's bit matching the frame number.
 */
#define FRAME_INDEX_IN_BITMAP(frame_num)    (frame_num / 32)
#define OFFSET_IN_FRAME(frame_num)   (frame_num % 32)

bitmap_t* frames;   // bitmap_t is used for efficiency.
uint32_t frames_number;

extern uint32_t placement_address;

typedef struct {
    uint32_t frame_number;
    uint32_t frame_index_in_bitmap;
    uint32_t offset_in_frame;
} frame_info_t;


void alloc_frame(page_t* page, uint8_t kernel, uint8_t writable)
{
    if (NULL != page->frame_address) {
        // Frame is already allocated!
        return;
    }

    uint32_t index = find_first_available_frame();
    if (-ENOTFOUND == (int32_t)index) {
        PANIC("Failed to allocate frame - not available frames.");
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

static uint32_t find_first_available_frame()
{
    int frame_index, offset_in_frame = 0;
    for (frame_index = 0; frame_index < frames_number; ++frame_index) {
        if (is_bitmap_full(frames[frame_index])) {
            // Current bitmap is full, keep going.
            continue;
        }

        for (offset_in_frame = 0; offset_in_frame < sizeof(bitmap_t); ++offset_in_frame) {
            if (bitmap_test(frames[frame_index], offset_in_frame)) {
                return frame_index * sizeof(bitmap_t) + offset_in_frame;
            }    
        }
    }

    return -ENOTFOUND;
}

static void _fill_frame_info(uint32_t frame_address, frame_info_t* frame_info)
{
    uint32_t frame_number = frame_address / PAGESIZE;
    frame_info->frame_number = frame_number;
    frame_info->frame_index_in_bitmap = FRAME_INDEX_IN_BITMAP(frame_number);
    frame_info->offset_in_frame = offset_in_frame(frame_number);
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
