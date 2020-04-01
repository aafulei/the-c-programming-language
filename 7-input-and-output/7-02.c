/* Exercise 7-2. Write a program that will print arbitrary input in a sensible
                 way. As a minimum, it should print non-graphic characters in
                 octal or hexadecimal according to local custom, and break long
                 text lines. */

#include <ctype.h>
#include <stdio.h>

#define MAXLINE 79
#define OCTLEN 5

int main(void)
{
    int c, cnt = 0;
    while ((c = getchar()) != EOF) {
        if (isprint(c)) {
            if (cnt == MAXLINE) {
                putchar('\n');
                cnt = 0;
            }
            putchar(c);
            ++cnt;
        }
        else if (c == '\n') {
            putchar(c);
            cnt = 0;
        }
        else {
            if (cnt > MAXLINE - OCTLEN) {
                putchar('\n');
                cnt = 0;
            }
            printf("[%03o]", c);
            cnt += OCTLEN;
        }
    }
    return 0;
}
