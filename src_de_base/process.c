#include "inttypes.h"
#include <debug.h>
#include <string.h>
#include "process.h"

#define NUMBER_OF_REGISTERS 5
const char * process_state_name[] = {"Uninitialized", "Ready", "Chosen", "Sleeping", "Zombie"};
