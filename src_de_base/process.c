#include "inttypes.h"
#include <debug.h>
#include <string.h>
#include "process.h"
#include "scheduler.h"

#define NUMBER_OF_REGISTERS 5
const char * process_state_name[] = {"Uninitialized", "Ready", "Chosen", "Sleeping", "Zombie"};
extern int32_t g_number_of_processes_spawned;
extern struct Process g_process_control_block_table[];
extern struct Process * g_active_process;

/*
 * Creates a new process.
 * Leaving it ready to been executed
 * when chosen by the scheduler.
 *
 * Returns the process identifier (pid)
 * For more details: see `Lessons-pc-archi.md`
 */
int32_t new_process(char * name,  void (*process_fn)()) {
    // TODO: choose free slot policy is always pointing to the first cases first
    // Is it a desirable behavior?
    g_number_of_processes_spawned++;
    uint32_t free_place = 0;

    // Verify that the esp is not accessing addresses outside its dedicated stack
    // Verify at the beginning and at the end.

    assert(name != 0);
    assert(process_fn != 0);
    for (free_place = 1; free_place < MAX_NUM_OF_PROCESSES; free_place++){
        enum process_state process_state = g_process_control_block_table[free_place].state;
        if (process_state == UNINITIALIZED || process_state == ZOMBIE)
            break;
    }
    if (free_place  >= MAX_NUM_OF_PROCESSES)
        return -1;

    struct Process *process = &g_process_control_block_table[free_place];
    process->pid = g_number_of_processes_spawned;
    sprintf(process->name, "PROC %d", g_number_of_processes_spawned);
    process->state = READY;
    memset(process->register_table, 0, NUMBER_OF_REGISTERS * 4); // 4 bytes each register
    memset(process->call_stack, 0, PROCESS_STACK_SIZE * 4); // for bytes each stack case
    process->waking_time = 0;

    process->call_stack[PROCESS_STACK_SIZE-2] = (uint32_t)process_fn;
    process->call_stack[PROCESS_STACK_SIZE-1] = (uint32_t)end_process;
    process->register_table[ESP] = (uint32_t) &process->call_stack[PROCESS_STACK_SIZE-2];

    return process->pid;
}


void end_process() {
    g_active_process->state = ZOMBIE;
    schedule();
}
