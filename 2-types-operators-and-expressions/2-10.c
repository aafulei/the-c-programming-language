/* Exercise 2-10. Rewrite the function lower, which converts upper case
   letters to lower case, with a conditional expression instead of if-else. */

#include <stdio.h>

int lower(int c);

int main(void)
{
    int c;

    while ((c = getchar()) != EOF)
        putchar(lower(c));
    return 0;
}

int lower(int c)
{
    return 'A' <= c && c <= 'Z' ? c - 'A' + 'a' : c;
}
