/* Exercise 1-9. Write a program to copy its input to its output, replacing
   each string of one or more blanks by a single blank. */

#include <stdio.h>

#define NONBLANK 'A'

/* copy input to output, replacing consecutive blanks with a single blank */
int main(void)
{
    int c, lastc;

    lastc = NONBLANK;
    while ((c = getchar()) != EOF) {
        if (c != ' ' || lastc != ' ')
            putchar(c);
        lastc = c;
    }
}
