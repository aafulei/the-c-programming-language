/* Exercise 1-8. Write a program to count blanks, tabs, and newlines. */

#include <stdio.h>

/* count blanks, tabs, and newlines in input */
int main(void)
{
    int c, nb, nt, nl;

    nb = 0;
    nt = 0;
    nl = 0;
    while ((c = getchar()) != EOF) {
        if (c == ' ')
            ++nb;
        if (c == '\t')
            ++nt;
        if (c == '\n')
            ++nl;
    }
    printf("%d %d %d\n", nb, nt, nl);
}

/* Note: `else` has not been introduced yet; otherwise it should be used
   here. */
