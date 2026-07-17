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
    for (int i = 0; i < 4; i++) {
        printf("[time = %u] process: %s pid = %i\n", uptime(),
        name(), pid());
        sleep(2);
    }

    int32_t pid = new_process("PROC 11", proc1);
    if (pid == -1) {
        panic("Process table is full!");
    }
}


void proc2() {
    for (int i = 0; i < 2; i++) {
        printf("[time = %u] process: %s pid = %i\n", uptime(),
        name(), pid());
        sleep(10);
    }
    int32_t pid = new_process("PROC 12", proc1);
    if (pid == -1) {
        panic("Process table is full!");
    }
}

void proc3() {
    for (int i = 0; i < 10; i++) {
        printf("[time = %u] process: %s pid = %i\n", uptime(),
        name(), pid());
        sleep(5);
    }
    int32_t pid = new_process("PROC 13", proc1);
    if (pid == -1) {
        panic("Process table is full!");
    }
}

void kernel_start(void)
{
    clean_screen();
    printf("\n\n\n\n\n\nMalika, I implemented printf ;) habibi\n");
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
