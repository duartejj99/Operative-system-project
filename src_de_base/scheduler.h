#include "process.h"

void setup_scheduler();
void new_process(struct Process  *process, char * name,  void (*process_fn)());
void schedule();


char * name();
int32_t pid();
