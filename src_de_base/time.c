#include "segment.h"
#include "inttypes.h"
#include "screen.h"
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
const uint32_t* IDT_ADDR =(uint32_t *) 0x1000;

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

const uint8_t COMMAND_PORT = 0x20;

// const CLOCK_FREQ

// TODO:
//
// 1. IDT entry initialization on the index number 32.
// 2. Clock frequency initialization
// 3.Deactivate others interruptions while my interruption is running (This is necessary? it's not explicitely said)
// 4. Count until 1 sec
// 4.Activate back when my interruption finishes (said)
//

/*
 * Writes on the Upper right corner, the time since the system booted
 */
void write_time(char * time_as_string, uint32_t string_size) {
    // each string is one column on the
    place_cursor(0, SCREEN_WIDTH - string_size);
    printf(time_as_string, "%s");
}
