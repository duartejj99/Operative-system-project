/**
 * @file screen.h
 * @brief Terminal screen management
 *
 * Here are all the functions related to the terminal screen management.
 */

#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "inttypes.h"

extern const uint8_t SCREEN_WIDTH;

void write_char(uint32_t line, uint32_t col, char c);
void clean_screen();
void place_cursor(uint32_t line, uint32_t col);
void char_treatment(char c);
void scrolling();
#endif
