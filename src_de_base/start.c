#include "cpu.h"
#include "inttypes.h"
#include "screen.h"
#include "stdio.h"
#include "time.h"
#include "scheduler.h"

extern struct Process os_processes[];

void idle() {
    for (;;) {
        printf("[%s\t] pid = %i\n", name(), pid());
        // printf("[IDLE] I have the hand\n");
        schedule();
    }

}

void proc1() {
    for (;;) {
        printf("[%s\t] pid = %i\n", name(), pid());
        // printf("[PROC_1] I Have the hand\n");
        schedule();
    }
}

void kernel_start(void)
{
    clean_screen();
    printf("Malika, I implemented printf ;) habibi\n");
    // Interruption initialization
    init_pit_interruption_config();

    setup_scheduler();
    new_process(&os_processes[1], "PROC 1", proc1);

    idle();
    // sti();
    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
