#include "cpu.h"
#include "inttypes.h"
#include "screen.h"
#include "stdio.h"
#include "time.h"
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


void kernel_start(void)
{

    uint32_t x = fact(5);

    clean_screen();
    place_cursor(24,0);
    char_treatment('\n');
    //
    printf("Malika, I implemented printf ;) habibi");
    scrolling();
    printf("FAAA");
    char_treatment('\r');
    char_treatment('\f');
    printf("This is the value of x: %d", x);
    char * time = "HH:MM:SS";
    write_time(time, 8);
    tic_PIT();
    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
