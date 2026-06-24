/**
 * @file screen.h
 * @brief Terminal screen management
 *
 * Here are all the functions related to the terminal screen management.
 */

#include "inttypes.h"

void write_char(uint32_t line, uint32_t col, char c);
void clean_screen();
void place_cursor(uint32_t line, uint32_t col);
void char_treatment(char c);
