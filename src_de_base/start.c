#include "cpu.h"
#include "inttypes.h"
#include "screen.h"
#include "stdio.h"
#include "time.h"
#include "process.h"

extern void ctx_sw(int32_t * old_context, int32_t * new_context);
extern struct Process os_processes[];

void idle() {
    for (int i = 0; i < 3; i++) {
        printf("[IDLE] I want to give the hand to process 1\n");
        ctx_sw((int32_t *)&os_processes[0].register_table,(int32_t *)&os_processes[1].register_table);
        printf("[IDLE] Process 1 gave me the hand back\n");
    }

    printf("[IDLE] I block the system now\n");
    hlt();
}

void proc1() {
    for (;;) {
        printf("[PROC_1] I RECEIVED THE PROCESSOR from IDLE\n");
        printf("[PROC_1] I dont want it anyways\n");
        ctx_sw((int32_t *)&os_processes[1].register_table,(int32_t *)&os_processes[0].register_table);
    }
}

void kernel_start(void)
{
    clean_screen();
    printf("Malika, I implemented printf ;) habibi\n");
    // Interruption initialization
    init_pit_interruption_config();

    initialize_process(&os_processes[1], "proc_1", proc1);
    idle_process_initialization(&os_processes[0]);


    idle();
    // sti();
    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
