#include "inttypes.h"

void write_time(char * time_as_string, uint32_t string_size);
void tic_PIT();
void initialize_idt_entry(uint32_t it_number, void (*it_treatment_fn)(void));
void initialize_clk_frequency();
