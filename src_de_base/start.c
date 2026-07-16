#include "cpu.h"
#include "debug.h"
#include "inttypes.h"
#include "screen.h"
#include "stdio.h"
#include "time.h"
#include "scheduler.h"

void idle() {
    for (;;) {
        sti();
        hlt();
        cli();
    }

}

void proc1() {
    for (;;) {
        printf("[time = %u] process: %s pid = %i\n", uptime(),
        name(), pid());
        sleep(2);
    }
}


void proc2() {
    for (;;) {
        printf("[time = %u] process: %s pid = %i\n", uptime(),
        name(), pid());
        sleep(3);
    }
}

void proc3() {
    for (;;) {
        printf("[time = %u] process: %s pid = %i\n", uptime(),
        name(), pid());
        sleep(5);
    }
}

void kernel_start(void)
{
    clean_screen();
    printf("Malika, I implemented printf ;) habibi\n");
    // Interruption initialization
    init_pit_interruption_config();
    setup_scheduler();

    int32_t pid = new_process("PROC 1", proc1);
    if (pid == -1) {
        panic("Process table is full!");
    }

    pid = new_process("PROC 2", proc2);
    if (pid == -1) {
        panic("Process table is full!");
    }
    pid = new_process("PROC 3", proc3);
    if (pid == -1) {
        panic("Process table is full!");
    }
    idle();
    // sti(); // unmask interruptions
    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
