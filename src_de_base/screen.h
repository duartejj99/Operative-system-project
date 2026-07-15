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

void clean_screen();
void update_cursor_on_screen(uint32_t line, uint32_t col);
uint32_t cursor_line();
uint32_t cursor_column();

#endif
