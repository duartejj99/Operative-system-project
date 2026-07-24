#include "process.h"
#include "screen.h"
#include "string.h"
#include "debug.h"
#include "inttypes.h"
#include "time.h"
#include "stdio.h"
#include "scheduler.h"

extern void ctx_sw(int32_t * old_context, int32_t * new_context);

struct Process g_process_control_block_table[MAX_NUM_OF_PROCESSES];
struct Process * g_active_process = &g_process_control_block_table[0];
int32_t g_number_of_processes_spawned = 0;

static void wake_up_sleeping_processes();
static void display_processes_state();
static int32_t next_process_pid();
static void update_active_process_state(int32_t chosen_pid);

/*
 * Initialize idle process as the first active process
 */
void setup_scheduler() {
    struct Process * idle = &g_process_control_block_table[0];
    strcpy(idle->name, "IDLE 0");
    idle->pid = 0;
    idle->state = CHOSEN;
}

/*
 * Sleeps a process during a `number_of_seconds`.
 */
void sleep(uint32_t number_of_seconds) {
    g_active_process->waking_time = uptime() + number_of_seconds;
    g_active_process->state = SLEEPING;
    schedule();
}

void wake_up_sleeping_processes(){
    for(int pid = 1; pid < MAX_NUM_OF_PROCESSES; pid ++) {
        if (g_process_control_block_table[pid].state != SLEEPING)
            continue;

        if (g_process_control_block_table[pid].waking_time <= uptime()) {
            g_process_control_block_table[pid].state = READY;
        }
    }
}


/*
 * Returns the active process name
 */
char *name() {
    return g_active_process->name;
}

/*
 * Returns the active process pid
 */
int32_t pid() {
    return g_active_process->pid;
}

/*
 * Choose and run the next process on the machine,
 * saving the state of current process being executed
 * and restoring the chosen process state when it was stopped.
 *
 * This last part is known as a context switch.
 */
void schedule() {
    struct Process * current_process = g_active_process;

    wake_up_sleeping_processes();
    int32_t chosen_pid = next_process_pid();
    if (chosen_pid < 0) return;
    update_active_process_state(chosen_pid);
    display_processes_state();

    ctx_sw((int32_t *)current_process->register_table, (int32_t *)g_active_process->register_table);
}

void update_active_process_state(int32_t chosen_pid){
    // Current Process
    if (g_active_process->state == CHOSEN) // If Sleeping or Zombie don't put on Ready queue
        g_active_process->state = READY;  // active process
    // Next Process
    g_active_process = &g_process_control_block_table[chosen_pid]; // update active_process pointer
    g_active_process->state = CHOSEN;
}

int32_t next_process_pid() {
    int32_t chosen_process_pid = g_active_process->pid;
    int32_t process_counter = MAX_NUM_OF_PROCESSES;

    while(process_counter >= 0) {
        if (g_process_control_block_table[chosen_process_pid].state == READY)
            break;
        chosen_process_pid = (chosen_process_pid + 1) % MAX_NUM_OF_PROCESSES;
        process_counter--;
    }

    if (process_counter < 0) {
        // we didnt find an activable process.
        return -1;
    }
    return chosen_process_pid;
}

void display_processes_state(){
    char * name;
    const char * state;
    uint32_t line = cursor_line();
    uint32_t column = cursor_column();
    update_cursor_on_screen(0, 0);
    for (int i = 0; i < MAX_NUM_OF_PROCESSES; i++) {
        name = g_process_control_block_table[i].name;
        state = process_state_name[g_process_control_block_table[i].state];
        printf("[%s\t] pid = %i\tstate: %s\t\t\n", name, i, state);
    }
    update_cursor_on_screen(line, column);
}
