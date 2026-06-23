#include "cpu.h"
#include "inttypes.h"

const uint16_t VGA_DATA_PORT = 0x3D4;
const uint16_t VGA_COMMAND_PORT = 0x3D5;

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



void send_data_to_screen(uint16_t data) {
    uint8_t lo_data_byte = data & 0xFF;
    uint8_t hi_data_byte = (data >> 8) & 0xFF;
    outb(0x0F, VGA_COMMAND_PORT);
    outb(lo_data_byte, VGA_DATA_PORT);
    outb(0x0E, VGA_COMMAND_PORT);
    outb(hi_data_byte, VGA_DATA_PORT);
}

uint32_t *mem_ptr(uint32_t line, uint32_t col) {


    return (uint32_t *) (2 * (line * 80 + col) );
}

void write_char(uint32_t line, uint32_t col, char c) {
    uint32_t * mem_zone = mem_ptr(line, col);
    *mem_zone = (uint32_t)c;
}
