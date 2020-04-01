/* Exercise 1-20. Write a program detab that replaces tabs in the input with
   the proper number of blanks to space to the next tab stop. Assume a fixed
   set of tab stops, say every n columns. Should n be a variable or a symbolic
   parameter? */

/* Answer: Since n is fixed, it should be a symbolic parameter. */

#include <stdio.h>

#define TABSIZE 8

/* detab: replace tabs in input with proper number of blanks */
int main(void)
{
    int c, i, j;

    i = 0;  /* number of characters in current line */
    while ((c = getchar()) != EOF)
        if (c == '\t') {
            j = TABSIZE - i % TABSIZE;  /* number of blanks to next tab stop */
            i = i + j;                  /* update i before decrementing j */
            while (j > 0) {
                putchar(' ');
                --j;
            }
        }
        else {
            putchar(c);
            if (c == '\n')
                i = 0;
            else
                ++i;
        }
    return 0;
}
