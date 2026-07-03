#include "inttypes.h"
#include "stdbool.h"

void write_time(char * time_as_string, uint32_t string_size);
void tic_PIT();
void initialize_idt_entry(uint32_t it_number, void (*it_treatment_fn)(void));
void initialize_clk_frequency();
void mask_IRQ(uint32_t num_IRQ, bool mask);
