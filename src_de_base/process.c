#include "inttypes.h"

#define PROCESS_STACK_SIZE 512
enum process_state {
    CHOSEN,
    SLEEPING,
    ACTIVABLE,
};

struct Process {
    int pid;
    char name[20];
    enum process_state state;
    uint32_t register_table[5];
    int call_stack[PROCESS_STACK_SIZE];
};

// TODO: Right now is created locally, after going out of the function, the stack of this function will collapse and
// the struct memory wont be valid for using.
//
// I should use malloc here, or pass by pointer.
struct Process new_process() {
    struct Process p1 = {
        .pid = 1,
        .name = "process_dummy",
        .state = CHOSEN,
        .register_table = {0,0,0,0,0},
        .call_stack = {0},
    };

    return p1;
}
