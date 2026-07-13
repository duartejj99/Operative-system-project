#include "inttypes.h"
#include <debug.h>
#include <string.h>
#include "process.h"

#define NUMBER_OF_REGISTERS 5
int32_t pid = 0;
struct Process os_processes[5];


// TODO: Right now is created locally, after going out of the function, the stack of this function will collapse and
// the struct memory wont be valid for using.
//
// I should use malloc here, or pass by pointer.
void initialize_process(struct Process  *p2, char * name,  void (*process_fn)()) {
    assert(name != 0);
    pid++;
    strcpy(p2->name, name);
    p2->pid = pid;
    p2->state = READY;
    memset(p2->register_table, 0, NUMBER_OF_REGISTERS * 4);
    memset(p2->call_stack, 0, PROCESS_STACK_SIZE * 4);
    p2->call_stack[PROCESS_STACK_SIZE-1] = (uint32_t)process_fn;
    p2->register_table[ESP] = (uint32_t) &p2->call_stack[PROCESS_STACK_SIZE-1];
}

void idle_process_initialization(struct Process *p) {
    strcpy(p->name, "idle");
    p->pid = 0;
    p->state = CHOSEN;

}
