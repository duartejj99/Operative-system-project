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
        sleep(20);
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
        // bugged the third time  it passed through here (t = 18) why
        // To solve the bugg, the esp for some reason is set at os_processes+8280 instead of +8296 that should be the right value.
        // He ended up reading garbage and jumping the ip into a random address.
        // check the third sleep, and all the proc 3 sleeps THIS WAS THE KEY
        //
        // Stablished a watch on 0x119978 (os_processes+8280), this allowed me to know when this memory case
        // was rewritten
        //
        // FOUND BUG on new_process function.
        // Probably process name creation of proc 5 corrupts the stack of proc 3
        // Maybe not enough space
        //
        // Find alternatives
        // static variables, more stack size, STUDY <3
        sleep(6);
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
