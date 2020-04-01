/* Exercise 1-12. Write a program that prints its input one word per line. */

#include <stdio.h>

#define IN 1    /* init: has entered a non-blank char; state: inside a word */
#define OUT 0   /* init: has not entered a non-blank; state: outside a word */

int main(void)
{
    int c, init, state;

    init = state = OUT;
    while ((c = getchar()) != EOF) {
        if (c == ' ' || c == '\t' || c == '\n')
            state = OUT;
        else {
            if (init == IN && state == OUT)
                putchar('\n');
            putchar(c);
            init = state = IN;
        }
    }
}
