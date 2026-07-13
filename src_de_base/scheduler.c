#include "process.h"
#include "string.h"
#include "debug.h"
#include "inttypes.h"

#define MAX_NUM_OF_PROCESSES 5

extern void ctx_sw(int32_t * old_context, int32_t * new_context);

struct Process os_processes[MAX_NUM_OF_PROCESSES];
int32_t number_of_processes = 0;
struct Process * active_process = &os_processes[0];
static void idle_process_initialization(struct Process * p);

void setup_scheduler() {
    idle_process_initialization(&os_processes[0]);
}

void schedule() {
    int32_t chosen_process_pid = 0;
    struct Process * chosen_process;
    struct Process * current_process = active_process;

    // choose next process
    while(chosen_process_pid < MAX_NUM_OF_PROCESSES) {
        if (os_processes[chosen_process_pid].state == READY)
            break;
        chosen_process_pid++;
    }
    assert(chosen_process_pid < MAX_NUM_OF_PROCESSES);
    chosen_process = &os_processes[chosen_process_pid];

    // Update processes states
    active_process->state = READY;  // active process
    chosen_process->state = CHOSEN; // next active process
    active_process = &os_processes[chosen_process_pid]; // update active_process pointer

    ctx_sw((int32_t *)current_process->register_table, (int32_t *)chosen_process->register_table);
}

/*
 * Initialize a new process.
 * Leaving it ready to been executed
 * when chosen by the scheduler.
 *
 * For more details: see `Lessons-pc-archi.md`
 */
void new_process(struct Process  *process, char * name,  void (*process_fn)()) {
    assert(name != 0);
    assert(number_of_processes + 1 < MAX_NUM_OF_PROCESSES);
    number_of_processes++;

    strcpy(process->name, name);
    process->pid = number_of_processes;
    process->state = READY;
    memset(process->register_table, 0, NUMBER_OF_REGISTERS * 4);
    memset(process->call_stack, 0, PROCESS_STACK_SIZE * 4);
    process->call_stack[PROCESS_STACK_SIZE-1] = (uint32_t)process_fn;
    process->register_table[ESP] = (uint32_t) &process->call_stack[PROCESS_STACK_SIZE-1];
}

/*
 * Setup idle process data
 */
static void idle_process_initialization(struct Process *p) {
    strcpy(p->name, "idle");
    p->pid = 0;
    p->state = CHOSEN;
}
