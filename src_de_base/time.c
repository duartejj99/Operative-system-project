#include "cpu.h"
#include "inttypes.h"
#include "screen.h"
#include "segment.h"
#include "stdio.h"

/*
 * Interruption Description Table:
 * A table with a list of addresses that points
 * to different interruption function treatment.
 *
 * This constant is the address of this table.
 * We should initialize the entry that corresponds to
 * the interruption we want to manage.
 */
const uint32_t *IDT_ADDR = (uint32_t *)0x1000;

/*
 * Interruption configuration to mask other interruptions while
 * my interruption treatment is being executed.
 *
 * This means my interruption itself cannot be interrupted.
 */
const uint32_t IT_CONFIG = 0x8E00;

/*
 * Data port to read the current IRQ bitmap Mask
 * IRQ: Interruption ReQuest bitmap
 */
const uint8_t IRQ_MASK_DATA_PORT = 0x21;

/*
 * The interruption treated is the number 32 (0x20)
 */
const uint8_t CLK_IT_NUMBER = 0x20;
const uint16_t CLK_FREQ_HZ = 50;

const uint8_t IT_CONTROLLER_COMMAND_PORT = 0x20;

uint32_t clk_ticks = 0;
char *time_display = "";

// const CLOCK_FREQ

// TODO:
//
// 1. IDT entry initialization on the index number 32.
// 2. Clock frequency initialization
// 3.Deactivate others interruptions while my interruption is running (This is
// necessary? it's not explicitely said)
// 4. Count until 1 sec
// 4.Activate back when my interruption finishes (said)
//

/*
 * Writes on the Upper right corner, the time since the system booted
 */
void write_time(char *time_as_string, uint32_t string_size) {
    place_cursor(0, SCREEN_WIDTH - string_size);
    printf(time_as_string, "%s");
}

// If we are here is because the clk ticked
/*
 * Programmable Interval Timer Interruption treatment
 */
void tic_PIT() {
    // 1. We communicate to the Interruption controler we are treating the IT, so
    // it can listen to other ITs
    outb(CLK_IT_NUMBER, IT_CONTROLLER_COMMAND_PORT);
    clk_ticks++;

    uint8_t seconds = 10;
    uint8_t minutes = 12;
    uint8_t hours = 3;
    if (clk_ticks == CLK_FREQ_HZ) {
        seconds++;
        if (seconds == 60) {
            minutes++;
        }
        if (minutes == 60) {
            hours++;
            minutes = 0;
        }
    }

    sprintf(time_display, "%02d:%02d:%02d", hours, minutes, seconds);
    write_time(time_display, 8);

}

/*
 * Initializes the Interruption description table (IDT) entry.
 *
 * It writes on the interruption case identified by its number, the corresponding interruption treatment function pointer
 * with a fixed configuration.
 */
void initialize_idt_entry(uint32_t it_number, void (*it_treatment_fn)(void)) {
    // it_number should be between 0 and 256. There are only 256 interruptions on x86

    uint32_t * idt_entry = (uint32_t *)IDT_ADDR + it_number;
    uint32_t it_treatment_addr_lo = ((uint32_t)it_treatment_fn & 0xFF);
    uint32_t it_treatment_addr_hi = ((uint32_t)it_treatment_fn & 0xFF00);

    // 1st word
    *idt_entry = KERNEL_CS << 16 | it_treatment_addr_lo;
    idt_entry++;
    // 2nd word
    *idt_entry =  it_treatment_addr_hi | IT_CONFIG;
}
