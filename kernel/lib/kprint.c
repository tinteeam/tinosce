#include "kprint.h"
#include <stdint.h>

#define VIDEO_ADDRESS 0xB8000
#define MAX_COLS 80
#define MAX_ROWS 25
#define WHITE_ON_BLACK 0x0F

uint16_t* video_memory = (uint16_t*) VIDEO_ADDRESS;

int cursor_row = 0;
int cursor_col = 0;

void update_cursor() {
    uint16_t position = cursor_row * MAX_COLS + cursor_col;
    outb(0x3D4, 14);
    outb(0x3D5, position >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, position & 0xFF);
}

void kprint_char(char c) {
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
    } else if (c == '\b') {
        if (cursor_col > 0) {
            cursor_col--;
            int offset = cursor_row * MAX_COLS + cursor_col;
            video_memory[offset] = ' ' | (WHITE_ON_BLACK << 8);
        }
    } else {
        int offset = cursor_row * MAX_COLS + cursor_col;
        video_memory[offset] = c | (WHITE_ON_BLACK << 8);
        cursor_col++;
        if (cursor_col >= MAX_COLS) {
            cursor_col = 0;
            cursor_row++;
        }
    }

    if (cursor_row >= MAX_ROWS) {
        // Scroll screen up
        for (int row = 1; row < MAX_ROWS; row++) {
            for (int col = 0; col < MAX_COLS; col++) {
                video_memory[(row - 1) * MAX_COLS + col] = video_memory[row * MAX_COLS + col];
            }
        }

        // Clear the last line
        for (int col = 0; col < MAX_COLS; col++) {
            video_memory[(MAX_ROWS - 1) * MAX_COLS + col] = ' ' | (WHITE_ON_BLACK << 8);
        }

        cursor_row = MAX_ROWS - 1;
    }

    update_cursor();
}

void kprint(const char* str) {
    while (*str) {
        kprint_char(*str++);
    }
}

void kprint_clear() {
    for (int i = 0; i < MAX_COLS * MAX_ROWS; i++) {
        video_memory[i] = ' ' | (WHITE_ON_BLACK << 8);
    }

    cursor_row = 0;
    cursor_col = 0;
    update_cursor();
}

