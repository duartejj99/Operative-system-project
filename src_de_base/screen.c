#include "cpu.h"
#include "inttypes.h"
// #include <assert.h>

const uint16_t VGA_COMMAND_PORT = 0x3D4;
const uint16_t VGA_DATA_PORT = 0x3D5;

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

uint32_t *mem_ptr(uint32_t line, uint32_t col);
void write_char(uint32_t line, uint32_t col, char c);
void set_cursor_position(uint16_t cursor_position);


void place_cursor(uint32_t line, uint32_t col) {
  uint16_t cursor_position =  (line * 80 + col);
  set_cursor_position(cursor_position);
}

void clean_screen() {
    for (int l= 0; l < 25; l++) {
        for (int c = 0; c < 80; c++)
        write_char(l, c, ' ');
    }
}

/**
 * [15      ,   14 - 12,    11 - 8,         7 - 0         ]
 * [clignote -  bg color -  text color -    ASCII char code]
 */
void write_char(uint32_t line, uint32_t col, char c) {
    uint32_t * mem_zone = mem_ptr(line, col);
    // [0,  000,     1111]
    // [0,  WHITE,   BLACK]
    uint32_t char_format = 0x0F;    // high byte is the config
    *mem_zone = (char_format << 8) + (uint32_t)c; // Low byte is the char
}


void set_cursor_position(uint16_t cursor_position) {
    uint8_t lo_data_byte = cursor_position & 0xFF;
    uint8_t hi_data_byte = (cursor_position >> 8) & 0xFF;
    outb(0x0F, VGA_COMMAND_PORT);
    outb(lo_data_byte, VGA_DATA_PORT);
    outb(0x0E, VGA_COMMAND_PORT);
    outb(hi_data_byte, VGA_DATA_PORT);
}

uint32_t *mem_ptr(uint32_t line, uint32_t col) {
    // assert(line >= 0);
    // assert(line < 25);
    // assert(col >= 0);
    // assert(col < 80);
    return (uint32_t *) (0xB8000 + 2 * (line * 80 + col) );
}