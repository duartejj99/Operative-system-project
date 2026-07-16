#include "process.h"
#include "inttypes.h"

void setup_scheduler();
int32_t new_process(char * name,  void (*process_fn)());
void schedule();
void sleep(uint32_t number_of_seconds);
void end_process();

char * name();
int32_t pid();
