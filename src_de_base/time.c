#include "cpu.h"
#include "inttypes.h"
#include "screen.h"
#include "segment.h"
#include "stdio.h"
#include "stdbool.h"
#include "debug.h"

// PIT configuration:
/*
 * PIT default clock signal frequency on HZ
 */
const uint32_t QUARTZ = 0x1234DD;
/*
 * The desired PIT clock signal frequency on HZ we want to set
 */
const uint16_t CLK_FREQ_HZ = 50;

/*
 * The reload value communicated to the PIT to configure the frequency of
 * the clock signal.
 * This parameter is the calculated reload count value to be set
 * In order to obtain a clock signal with a frequency
 * of CLK_FREQ.
 *
 * The PIT will count internally until this value and raise an interruption
 * at a frequency of CLK_FREQ_HZ.
 * freq = QUARTZ / PIT_RELOAD_VALUE
 */
const uint16_t PIT_RELOAD_VALUE = QUARTZ/CLK_FREQ_HZ;

/*
 * PIT Input/Output Mode/Command port
 *
 * Here you configure on the register in which mode
 * the PIT is going to operate and in which channel.
 *
 * For more info:
 * https://wiki.osdev.org/Programmable_Interval_Timer
 */
const uint8_t PIT_IO_COMMAND_PORT = 0x43;
/*
 * Mode/Command register configuration.
 *
 * This configuration specifies the PIT is going to
 * operate as a frequency divider (mode 2) on channel 0.
 *
 * And that we are going to pass a frequency divisor of 16 bits
 * length.
 *
 * Note: Channel 0 is connected to IRQ0
 *
 * For more info:
 * https://wiki.osdev.org/Programmable_Interval_Timer
 *
 */
const uint8_t PIT_IO_COMMAND_DATA = 0x34;

/* PIT Input/Output port corresponding to Channel 0
 * This channel is connected to IRQ0 (Interruption controller)
 */
const uint8_t PIT_IO_CHANNEL_0 = 0x40;


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
const uint8_t IT_CONTROLLER_COMMAND_PORT = 0x20;
/*
 * Number of clock ticks since the last second past.
 *
 * A second is pass when the clk has ticked CLK_FREQ_HZ times.
 */
uint32_t clk_ticks = 0;
char time_display[15];
uint8_t seconds = 0;
uint8_t minutes = 0;
uint8_t hours = 0;

/*
 * Writes on the Upper right corner, the time since the system booted
 */
void write_time(char *time_as_string, uint32_t string_size) {
    place_cursor(0, SCREEN_WIDTH - string_size);
    printf(time_as_string, "%s");
}

/*
 * Programmable Interval Timer Interruption treatment
 *
 * The interruption is sent from the interruption controller
 * to the processor.
 */
void tic_PIT() {
    // 1. We communicate to the Interruption controler we are treating the IT, so
    // it can listen to other ITs
    outb(CLK_IT_NUMBER, IT_CONTROLLER_COMMAND_PORT);
    clk_ticks++;

    if (clk_ticks == CLK_FREQ_HZ/3) {
        clk_ticks = 0;
        seconds++;
        if (seconds == 60) {
            minutes++;
            seconds = 0;
        }
        if (minutes == 60) {
            hours++;
            minutes = 0;
        }
        sprintf(time_display, "%02d:%02d:%02d", hours, minutes, seconds);
        write_time(time_display, 8);
    }
}

/*
 * Initializes the Interruption description table (IDT) entry.
 *
 * It writes on the interruption case identified by its number, the corresponding interruption treatment function pointer
 * with a fixed configuration.
 */
void initialize_idt_entry(uint32_t it_number, void (*it_treatment_fn)(void)) {
    // it_number should be between 0 and 256. There are only 256 interruptions on x86
    assert(it_number >= 0);
    assert(it_number <= 255);
    // each entry occupies 8 bytes so the index should inc/dec each 8 bytes.
    uint32_t * idt_entry = (uint32_t *)IDT_ADDR + it_number * 2;
    uint32_t it_treatment_addr_lo = ((uint32_t)it_treatment_fn & 0xFFFF);
    uint32_t it_treatment_addr_hi = ((uint32_t)it_treatment_fn & 0xFFFF0000);

    // 1st word
    *idt_entry = KERNEL_CS << 16 | it_treatment_addr_lo;
    idt_entry++;
    // 2nd word
    *idt_entry =  it_treatment_addr_hi | IT_CONFIG;
}

/*
 * Configure the PIT signal frequency emmition.
 *
 * Normally it emits a signal with a `QUARTZ` frequency.
 * Here we configure it to emit a signal with a `CLK_FREQ_HZ` frequency.
 *
 * In this case the constants configure the clock to do so on PIT channel 0.
 *
 * For more info, see `PIT_IO_COMMAND_DATA` description
 */
void initialize_clk_frequency() {
    outb(PIT_IO_COMMAND_DATA, PIT_IO_COMMAND_PORT);
    outb(PIT_RELOAD_VALUE & 0xFF, PIT_IO_CHANNEL_0);
    outb(((PIT_RELOAD_VALUE & 0xFF00) >> 8), PIT_IO_CHANNEL_0);

    sprintf(time_display, "%02d:%02d:%02d", hours, minutes, seconds);
}

/*
 * Mask/Unmask the interruption number `num_IRQ`
 * on the Programmable interruption controller.
 *
 * A mask set to true, means the controller ignores the interruption.
 * Otherwise, it communicates the interruption to the processor.
 */
void mask_IRQ(uint32_t num_IRQ, bool mask) {
    uint8_t irq_bitmap_mask = inb(IRQ_MASK_DATA_PORT);
    uint8_t mask_to_apply;
    if (mask) {
        mask_to_apply = 1 << num_IRQ;
        irq_bitmap_mask = irq_bitmap_mask | mask_to_apply;
    }else{
        mask_to_apply = ~(1<<num_IRQ);
        irq_bitmap_mask = irq_bitmap_mask & mask_to_apply;
    }

    outb(irq_bitmap_mask, IRQ_MASK_DATA_PORT);
}
