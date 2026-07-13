#include "cpu.h"
#include "inttypes.h"
#include "screen.h"
#include "stdio.h"
#include "time.h"
#include "process.h"

extern void ctx_sw(int32_t * old_context, int32_t * new_context);
extern struct Process os_processes[];

// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display

// une fonction bien connue
uint32_t fact(uint32_t n)
{
    uint32_t res;
    if (n <= 1) {
        res = 1;
    } else {
        res = fact(n - 1) * n;
    }
    return res;
}

void idle() {
    printf("[IDLE] I want to give the hand to process 1\n");
    ctx_sw((int32_t *)&os_processes[0].register_table,(int32_t *)&os_processes[1].register_table);
}

void proc1() {
    printf("[PROC_1] I RECEIVED THE PROCESSOR\n");
    printf("[PROC_1] I dont want it anyways\n");
    hlt();
}

void kernel_start(void)
{
    clean_screen();
    printf("Malika, I implemented printf ;) habibi\n");
    // Interruption initialization
    init_pit_interruption_config();

    // struct Process process1, idle_proc;
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
