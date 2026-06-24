#include "cpu.h"
#include "inttypes.h"
#include "screen.h"
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
    int ptr_size = sizeof(uint32_t *);
    int data_size = sizeof(uint32_t);

    uint32_t x = fact(5);
    // char c = 'j';

    clean_screen();
    // write_char(10,20,c);
    // write_char(10,21,c);
    // place_cursor(10,21);
    char_treatment('M');
    char_treatment('a');
    char_treatment('l');
    char_treatment('i');
    char_treatment('k');
    char_treatment('a');
    char_treatment(' ');
    char_treatment('m');
    char_treatment('i');
    char_treatment(' ');
    char_treatment('a');
    char_treatment('m');
    char_treatment('o');
    char_treatment('r');
    char_treatment('\b');
    char_treatment('\n');
    char_treatment('\t');
    char_treatment('\t');
    char_treatment('\r');
    char_treatment('\f');

    // place_cursor(0,1);
    // place_cursor(0,2);
    // place_cursor(0,3);
    // place_cursor(0,4); // something happens here
    // place_cursor(0,5);
    // place_cursor(0,6);


    // quand on saura gerer l'ecran, on pourra afficher x
    (void)x;
    (void)ptr_size;
    (void)data_size;

    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
