#include "cpu.h"
#include "inttypes.h"
#include "screen.h"
#include "stdio.h"
#include "time.h"
// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display

extern void traitant_IT_32();
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
    clean_screen();
    place_cursor(24,0);
    char_treatment('\n');
    printf("Malika, I implemented printf ;) habibi");
    initialize_idt_entry(32, traitant_IT_32);
    mask_IRQ(0, false);


    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
