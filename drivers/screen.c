#pragma once

#include "../arch/x86/io.h"
#include "screen.h"
#include "../kernel/util.h"

#define VIDEO_CONTROLLER_ADDRESS (0xb8000)
#define MAX_ROWS (25)
#define MAX_COL (80)
#define WHITE_ON_BLACK (0x0f)
#define RED_ON_BLACK (0xf4)

#define REG_SCREEN_CTRL (0x3d4)
#define REG_SCREEN_DATA (0x3d5)

#define DATA_REG_REQUEST_HIGH_BYTE_CODE (14)
#define DATA_REG_REQUEST_LOW_BYTE_CODE (15)

#define NEWLINE_CHARACTER ('\n') 

typedef uint32_t screen_offset_t;

/* Declaration of private functions */
screen_offset_t _print_char(uint8_t character, uint32_t col, uint32_t row, uint8_t attributes);
screen_offset_t _get_cursor_offset();
void _set_cursor_offset(screen_offset_t offset);
screen_offset_t _get_offset(uint32_t row, uint32_t col);
screen_offset_t _get_offset_row(uint32_t offset);
screen_offset_t _get_offset_col(uint32_t offset);
screen_offset_t _handle_scrolling();

/*** Public kernel API ***/

void kprint_at(const uint8_t* message, int32_t col, int32_t row)
{
    _kprint_at(message, col, row, WHITE_ON_BLACK);
}  

void kprint(const uint8_t* message) 
{
    kprint_at(message, -1, -1);
}

void kprint_err(const uint8_t* message)
{
    _kprint_at(message, -1, -1, RED_ON_BLACK);
}

void kclear_screen()
{
    uint32_t row = 0, col = 0;

    for (row = 0; row < MAX_ROWS; ++row) {
        for (col = 0; col < MAX_COL; ++col) {
            _print_char(' ', col, row, WHITE_ON_BLACK);
        }
    }

    _set_cursor_offset(_get_offset(0, 0));
}

/*** Private helper functions ***/

void _kprint_at(const uint8_t* message, int32_t col, int32_t row, uint8_t attributes)
{
    screen_offset_t offset = 0;
    if (col >= 0 && row >= 0) {
        offset = _get_offset(row, col);
    } else {
        offset = _get_cursor_offset();
        col = _get_offset_col(offset);
        row = _get_offset_row(offset);
    }

    uint32_t i = 0;
    while (0x00 != message[i]) {
        offset = _print_char(message[i], col, row, attributes);
        col = _get_offset_col(offset);
        row = _get_offset_row(offset);
        ++i;
    }
} 

uint32_t _print_char(uint8_t character, uint32_t col, uint32_t row, uint8_t attributes)
{
    uint8_t* video_memory = (uint8_t*) VIDEO_CONTROLLER_ADDRESS;

    // If no attributes specified, use default.
    if (!attributes) {
        attributes = WHITE_ON_BLACK;
    }

    // If column and row specified, use them.
    // Otherwise - use current cursor offset.
    screen_offset_t offset = 0;
    if (col >= 0 && row >= 0) {
        offset = _get_offset(row, col);
    } else {
        offset = _get_cursor_offset();
    }

    if (NEWLINE_CHARACTER == character) {
        row = _get_offset_row(offset);
        offset = _get_offset(row + 1, 0);
    } else {
        video_memory[offset] = character;
        video_memory[offset + 1] = attributes;
        offset += 2;
    }

    if (offset >= MAX_ROWS * MAX_COL * 2) {
        offset = _handle_scrolling(offset);
    }

    _set_cursor_offset(offset);
    return offset;
} 

screen_offset_t _handle_scrolling(screen_offset_t offset)
{
    uint32_t i = 1;  // Must start with the first row
    for (i; i < MAX_ROWS; ++i) {
        memcopy( VIDEO_CONTROLLER_ADDRESS + _get_offset(i, 0),
                VIDEO_CONTROLLER_ADDRESS + _get_offset(i - 1, 0),
                    MAX_COL * 2 );
    }

    // Since each row was predecessed by its following, we need to blank the last row.
    uint8_t* last_row = VIDEO_CONTROLLER_ADDRESS + _get_offset(MAX_ROWS - 1, 0);
    for (i = 0; i < MAX_COL * 2; ++i) {
        last_row[i] = 0;
    }

    return offset - 2 * MAX_COL;
}

uint32_t _get_cursor_offset() 
{
    // Use VGA ports to read current cursor offset.
    // 14 (DATA_REG_REQUEST_HIGH_BYTE_CODE) asks for the high byte of the cursor offset.
    // 15 (DATA_REG_REQUEST_LOW_BYTE_CODE) asks for the lower byte of the cursor offset.
    outb(REG_SCREEN_CTRL, DATA_REG_REQUEST_HIGH_BYTE_CODE);
    screen_offset_t offset = inb(REG_SCREEN_DATA) << 8;
    outb(REG_SCREEN_CTRL, DATA_REG_REQUEST_LOW_BYTE_CODE);
    offset += inb(REG_SCREEN_DATA);
    return offset * 2;
}

void _set_cursor_offset(screen_offset_t offset) 
{
    // Similar to get_cursor_offset, but instead of reading from the port, we will write to it.
    offset = offset / 2;
    outb(REG_SCREEN_CTRL, DATA_REG_REQUEST_HIGH_BYTE_CODE);
    outb(REG_SCREEN_DATA, (uint8_t)(offset >> 8));
    outb(REG_SCREEN_CTRL, DATA_REG_REQUEST_LOW_BYTE_CODE);
    outb(REG_SCREEN_DATA, (uint8_t)(offset & 0xFF));
}

screen_offset_t _get_offset(uint32_t row, uint32_t col) { return 2 * (row * MAX_COL + col); }
screen_offset_t _get_offset_row(screen_offset_t offset) { return (offset / (2 * MAX_COL)); }
screen_offset_t _get_offset_col(screen_offset_t offset) { return (offset - (_get_offset_row(offset) * 2 * MAX_COL)) / 2; }