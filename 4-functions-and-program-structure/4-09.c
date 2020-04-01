/* Exercise 4-9. Our getch and ungetch do not handle a pushed-back EOF
   correctly. Decide what their properties ought to be if an EOF is pushed
   back, then implement your design. */

/* See Section 4.3 (page 79) for the original getch() and ungetch().
    #define BUFSIZE 100

    char buf[BUFSIZE];
    int bufp = 0;

    int getch(void)
    {
        return (bufp > 0) ? buf[--bufp] : getchar();
    }

    void ungetch(int c)
    {
        if (bufp >= BUFSIZE)
            printf("ungetch: too many characters\n");
        else
            buf[bufp++] = c;
    } */

/* Answer: Our original ungetch() cannot properly handle EOF. Since buf is an
   array of char, and in C language char type can either be signed or unsigned,
   it cannot store EOF (an int that usually has a value of -1) in a portable
   way. In the calling functions, we intentionally use
    if (c != EOF)
        ungetch(c);
   to avoid such a scenario. In fact, we can just make buf an array of int, so
   that EOF can be pushed back to the buffer as a normal character. */

#include <stdio.h>

#define BUFSIZE 100

int buf[BUFSIZE];   /* modified: buf is now an array of int, not of char */
int bufp = 0;

int getch(void);
void ungetch(int);

/* test */
int main(void)
{
    ungetch(EOF);
    if (getch() == EOF)
        printf("Got an EOF!\n");
    return 0;
}

/* get a (possibly pushed-back) character */
int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

/* push character back on input */
void ungetch(int c)
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}
