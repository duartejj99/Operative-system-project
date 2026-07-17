#include "process.h"
#include "screen.h"
#include "string.h"
#include "debug.h"
#include "inttypes.h"
#include "time.h"
#define MAX_NUM_OF_PROCESSES 4

extern void ctx_sw(int32_t * old_context, int32_t * new_context);

struct Process os_processes[MAX_NUM_OF_PROCESSES];
int32_t number_of_processes = 0;
struct Process * active_process = &os_processes[0];
static void idle_process_initialization(struct Process * p);
static void wake_up_sleeping_processes();
static void display_processes_state();

void schedule() {
    int32_t chosen_process_pid = 0;
    struct Process * chosen_process;
    struct Process * current_process = active_process;

    wake_up_sleeping_processes();

    // choose next process
    while(chosen_process_pid < MAX_NUM_OF_PROCESSES) {
        if (os_processes[chosen_process_pid].state == READY)
            break;
        chosen_process_pid++;
    }
    // we didnt find an activable process.
    if (chosen_process_pid >= MAX_NUM_OF_PROCESSES) {
        return;
    }
    assert(chosen_process_pid < MAX_NUM_OF_PROCESSES);
    chosen_process = &os_processes[chosen_process_pid];

    // Update processes states
    if (active_process->state != SLEEPING && active_process->state != ZOMBIE)
        active_process->state = READY;  // active process
    chosen_process->state = CHOSEN; // next active process
    active_process = &os_processes[chosen_process_pid]; // update active_process pointer
    display_processes_state();
    ctx_sw((int32_t *)current_process->register_table, (int32_t *)chosen_process->register_table);
}

/*
 * Creates a new process.
 * Leaving it ready to been executed
 * when chosen by the scheduler.
 *
 * Returns the process identifier (pid)
 * For more details: see `Lessons-pc-archi.md`
 */
int32_t new_process(char * name,  void (*process_fn)()) {
    assert(name != 0);
    assert(process_fn != 0);
    if (number_of_processes + 1 >= MAX_NUM_OF_PROCESSES)
        return -1;
    number_of_processes++;

    struct Process *process = &os_processes[number_of_processes];
    *process =(struct Process){
        .pid = number_of_processes,
        .state = READY,
        .register_table = {0,0,0,0,0},
        .call_stack = {0},
        .waking_time = 0,
    };
    strcpy(process->name, name);
    process->call_stack[PROCESS_STACK_SIZE-1] = (uint32_t)process_fn;
    process->register_table[ESP] = (uint32_t) &process->call_stack[PROCESS_STACK_SIZE-1];

    return process->pid;
}

void end_process() {
    active_process->state = ZOMBIE;
    schedule();
}

void setup_scheduler() {
    idle_process_initialization(&os_processes[0]);
}

/*
 * Setup idle process data
 */
static void idle_process_initialization(struct Process *p) {
    strcpy(p->name, "IDLE");
    p->pid = 0;
    p->state = CHOSEN;
}

void sleep(uint32_t number_of_seconds) {
    active_process->waking_time = uptime() + number_of_seconds;
    active_process->state = SLEEPING;
    schedule();
}

void wake_up_sleeping_processes(){
    for(int pid = 1; pid < MAX_NUM_OF_PROCESSES; pid ++) {
        if (os_processes[pid].state != SLEEPING)
            continue;

        if (os_processes[pid].waking_time <= uptime()) {
            os_processes[pid].state = READY;
        }
    }
}

void display_processes_state(){
    char * name;
    const char * state;
    uint32_t line = cursor_line();
    uint32_t column = cursor_column();
    update_cursor_on_screen(0, 0);
    for (int i = 0; i < MAX_NUM_OF_PROCESSES; i++) {
        name = os_processes[i].name;
        state = process_state_name[os_processes[i].state];
        printf("[%s\t] pid = %i\tstate: %s\t\n", name, i, state);
    }
    update_cursor_on_screen(line, column);
}

/*
 * Returns the active process name
 */
char *name() {
    return active_process->name;
}

/*
 * Returns the active process pid
 */
int32_t pid() {
    return active_process->pid;
}
