#include "inttypes.h"
#include "screen.h"
#include "stdio.h"
#include "time.h"
#include "pit_interrupt.h"

static char time_display[15];
/* Time since system boot in seconds */
uint32_t uptime_in_seconds = 0;
static uint8_t seconds = 0;
static uint8_t minutes = 0;
static uint8_t hours = 0;
void increment_timer_in_one_sec();
static void display_time_on_screen(char *time_as_string, uint32_t string_size);

void start_system_time() {
    init_pit_interruption_config(increment_timer_in_one_sec);
    sprintf(time_display, "%02d:%02d:%02d", hours, minutes, seconds);
}

/*
 * Increment the timer in one second,
 * and update it on the screen.
 */
void increment_timer_in_one_sec() {
    uptime_in_seconds++;
    // 3600s in an hour= 60 min * 60 s
    hours = uptime_in_seconds / 3600;
    minutes = (uptime_in_seconds % 3600) / 60;
    seconds = (uptime_in_seconds % 3600) % 60;
    sprintf(time_display, "%02d:%02d:%02d", hours, minutes, seconds);
    display_time_on_screen(time_display, 8);
}

/* Return the time that has passed since
 * the system booting.
 *
 * The time is expressed in seconds
 */
uint32_t uptime() {
    return uptime_in_seconds;
}

/*
 * Writes on the Upper right corner, the time since the system booted
 */
static void display_time_on_screen(char *time_as_string, uint32_t string_size) {
    uint32_t line = cursor_line();
    uint32_t col = cursor_column();
    update_cursor_on_screen(0, SCREEN_WIDTH - string_size);
    printf(time_as_string, "%s");
    update_cursor_on_screen(line, col);
}
