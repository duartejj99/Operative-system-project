#include "inttypes.h"

#define MAX_NUM_OF_PROCESSES 5

void setup_scheduler();
void schedule();
void sleep(uint32_t number_of_seconds);

char * name();
int32_t pid();
