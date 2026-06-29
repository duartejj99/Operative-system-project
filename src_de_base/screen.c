#include "cpu.h"
#include <inttypes.h>
#include <string.h>
#include <screen.h>

const uint16_t VGA_COMMAND_PORT = 0x3D4;
const uint16_t VGA_DATA_PORT = 0x3D5;

const uint8_t SCREEN_WIDTH = 80;
const uint8_t SCREEN_HEIGHT = 25;

// Here I will detail the specifications of the screen we are working with.
// The screen is a 80 col x 25 lines grid. [25 lines x 80 cols] = 2000 cells
// Each cell is composed by 2 bytes : 1 byte for the character + 1 byte for the format

// The Memory map zone of the screen is
// 0x000B8000 - 0x000BFFFF
// When writing on this zone, the screen will display the characters on the screen.
//
// Each character is represented by a 16-bit value, where the lower 8 bits are the ASCII code and the upper 8 bits are the color attributes.
//
// [15      ,   14 - 12,    11 - 8,         7 - 0         ]
// [clignote -  bg color -  text color -    ASCII char code]
//
// bit 15 always to zero, the clignotement doesnt work well.
//
//

uint32_t cursor_line = 0, cursor_col = 0;

uint16_t *mem_ptr(uint32_t line, uint32_t col);
void write_char(uint32_t line, uint32_t col, char c);
void set_cursor_position(uint16_t cursor_position);
void place_cursor(uint32_t line, uint32_t col);
void clean_screen();
void char_treatment(char c);

void console_putbytes(const char *s, int len) {
    for (int i = 0; i < len; i++)
        char_treatment(s[i]);
}

void scrolling() {
    void * screen_part_to_copy = mem_ptr(1, 0);
    void * screen_begin = mem_ptr(0, 0);
    uint32_t screen_data_size_to_copy = (SCREEN_HEIGHT - 1) * SCREEN_WIDTH * 2;
    memmove(screen_begin, screen_part_to_copy, screen_data_size_to_copy);
    // delete the last screen line
    for (int col = 0; col < SCREEN_WIDTH; col++){
        write_char(SCREEN_HEIGHT - 1, col, ' ');
    }

    // Should I move the cursor or not? serious question.
    if(cursor_line != 0)
        place_cursor(cursor_line - 1, cursor_col);
    else
        place_cursor(0, 0);
}

void char_treatment(char c) {
    // from 0 to 31 control characters and 127
    // ASCII code from 32 to 126
    if (c >= 32 && c <= 126) {
        write_char(cursor_line, cursor_col, c);

        if (cursor_col == SCREEN_WIDTH - 1) {
            if (cursor_line == SCREEN_HEIGHT - 1)
                cursor_line = 0;
            cursor_line++;
            cursor_col = 0;
        }
        cursor_col = (cursor_col + 1) % 80;
    }

    // Treat control characters
    if (c >= 0 && c <= 31) {
        // 8, 9, 10, 12, 13
        switch (c) {
            // Backspace BS
            case 8: {
                if (cursor_col == 0)
                    break;

                cursor_col--;
                // write_char(cursor_line, cursor_col,' ');
                break;
            }
            // Horizontal tab HT
            case 9: {
                cursor_col = cursor_col + (8 - (cursor_col % 8));

                if (cursor_col == SCREEN_WIDTH) {
                    cursor_line++;
                    cursor_col = 0;
                }
                break;
            }
            // Line feed LF
            case 10: {
                cursor_col = 0;
                cursor_line = (cursor_line + 1) % SCREEN_HEIGHT;
                break;
            }
            // Form feed FF
            case 12: {
                clean_screen();
                cursor_col = 0;
                cursor_line = 0;
                break;
            }
            // Carriage return CR
            case 13: {
                cursor_col = 0;
                break;
            }

            default:{
                // The other cases are not treated, and are silently ignored
            }
        }
    }

    place_cursor(cursor_line, cursor_col);
}


void place_cursor(uint32_t line, uint32_t col) {
    cursor_line = line;
    cursor_col = col;
    uint16_t cursor_position =  (line * SCREEN_WIDTH + col);
    set_cursor_position(cursor_position);
}

void clean_screen() {
    for (int l= 0; l < SCREEN_HEIGHT; l++) {
        for (int c = 0; c < SCREEN_WIDTH; c++)
        write_char(l, c, ' ');
    }
    place_cursor(0, 0);
}

/**
 * [15      ,   14 - 12,    11 - 8,         7 - 0         ]
 * [clignote -  bg color -  text color -    ASCII char code]
 */
void write_char(uint32_t line, uint32_t col, char c) {
    uint16_t * mem_zone = mem_ptr(line, col);
    // [0,  000,     1111]
    // [0,  WHITE,   BLACK]
    uint16_t char_format = 0x0F;    // high byte is the config
    *mem_zone = (char_format << 8) + (uint8_t)c; // Low byte is the char
}


void set_cursor_position(uint16_t cursor_position) {
    uint8_t lo_data_byte = cursor_position & 0xFF;
    uint8_t hi_data_byte = (cursor_position >> 8) & 0xFF;
    outb(0x0E, VGA_COMMAND_PORT);
    outb(hi_data_byte, VGA_DATA_PORT);
    outb(0x0F, VGA_COMMAND_PORT);
    outb(lo_data_byte, VGA_DATA_PORT);
}

uint16_t *mem_ptr(uint32_t line, uint32_t col) {
    // assert(line >= 0);
    // assert(line < 25);
    // assert(col >= 0);
    // assert(col < 80);
    return (uint16_t *) (0xB8000 + 2 * (line * SCREEN_WIDTH + col) );
}
