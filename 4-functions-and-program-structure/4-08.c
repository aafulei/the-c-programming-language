/* Exercise 4-8. Suppose that there will never be more than one character of
   pushback. Modify getch and ungetch accordingly. */

#include <stdio.h>

int buffer = EOF;

int getch(void);
void ungetch(int);

/* test: get a char, unget it to buffer, and re-get it */
int main(void)
{
    int c;

    c = getch();
    printf("ungetch(%c)\n", c);
    ungetch(c);
    printf("re-get %c\n", getch());
    return 0;
}

/* modified getch(): get a character from a one-char buffer */
int getch(void)
{
    int c;

    if (buffer == EOF)
        return getchar();
    else {
        c = buffer;
        buffer = EOF;
        return c;
    }
}

/* modified ungetch(): push a character back to one-char buffer */
void ungetch(int c)
{
    if (buffer != EOF)
        printf("ungetch: too many characters\n");
    else
        buffer = c;
}
