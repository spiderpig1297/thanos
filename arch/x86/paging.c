#include "paging.h"

#include "../../kernel/kheap.h"
#include "../../kernel/bitmap.h"

/**
 * Index: the index in the array of bitmap_t.
 * Offset: the offset of the bitmap_t's bit matching the frame number.
 */
#define FRAME_INDEX_IN_BITMAP(frame_num)    (frame_num / 32)
#define FRAME_OFFSET_IN_BITMAP(frame_num)   (frame_num % 32)

bitmap_t* frames;   // bitmap_t is used for efficiency.
uint32_t frames_number;

extern uint32_t placement_address;

typedef struct {
    uint32_t frame_number;
    uint32_t frame_index_in_bitmap;
    uint32_t frame_offset_in_bitmap;
} frame_info_t;

static void fill_frame_info(uint32_t frame_address, frame_info_t* frame_info)
{
    uint32_t frame_number = frame_address / PAGESIZE;
    frame_info->frame_number = frame_number;
    frame_info->frame_index_in_bitmap = FRAME_INDEX_IN_BITMAP(frame_number);
    frame_info->frame_offset_in_bitmap = FRAME_OFFSET_IN_BITMAP(frame_number);
}

static void set_frame(uint32_t frame_address)
{
    frame_info_t frame_info  = { 0 };
    fill_frame_info(frame_address, &frame_info);
    bitmap_set( &frames[frame_info.frame_index_in_bitmap], 
                frame_info.frame_offset_in_bitmap);
}

static void clear_frame(uint32_t frame_address)
{
    frame_info_t frame_info  = { 0 };
    fill_frame_info(frame_address, &frame_info);
    bitmap_clear(&frames[frame_info.frame_index_in_bitmap], 
                 frame_info.frame_offset_in_bitmap);
}

static uint32_t test_frame(uint32_t frame_address)
{
    frame_info_t frame_info  = { 0 };
    fill_frame_info(frame_address, &frame_info);
    return bitmap_test( &frames[frame_info.frame_index_in_bitmap], 
                        frame_info.frame_offset_in_bitmap);
}