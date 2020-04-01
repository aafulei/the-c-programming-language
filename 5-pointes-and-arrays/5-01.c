/* Exercise 5-1. As written, getint treats a + or - not followed by a digit as
   a valid representation of zero. Fix it to push such a character back on the
   input. */

#include <ctype.h>
#include <stdio.h>

#define BUFSIZE 100

static char buf[BUFSIZE];
static int bufp = 0;

int getint(int *);
int getch(void);
void ungetch(int);

/* test: try to get an int from input, print result and next char */
int main(void)
{
    int a;

    switch (getint(&a)) {
        case 0: printf("not a number\n"); break;
        case EOF: printf("end of file\n"); return 0;
        default: printf("%d\n", a);
    }
    printf("next char: ");
    // Note:
    // printf(isgraph(a=getch()) ? "%c\n" : "[%d]\n", a);
    // has undefined behavior!
    a = getch();
    printf(isgraph(a) ? "%c\n" : "[%d]\n", a);
    return 0;
}

/* getint: get next integer from input into *pn (with fix on +/-) */
/* return (1) a positive number if input contains a valid number,
          (2) 0 if next input is not a number,
          (3) EOF for end of file. */
int getint(int *pn)
{
    int c, sign;

    while (isspace(c=getch()))
        ;
    if (c == EOF)
        return EOF;
    if (!isdigit(c) && c != '+' && c != '-') {
        ungetch(c);
        return 0;
    }
    sign = c == '-' ? -1 : 1;
    if ((c == '+' || c == '-') && !isdigit(c=getch())) {
        if (c != EOF)
            ungetch(c);
        ungetch(sign == 1 ? '+' : '-');
        return 0;
    }
    for (*pn = 0; isdigit(c); c = getch())
        *pn = *pn * 10 + (c-'0');
    *pn *= sign;
    if (c != EOF)
        ungetch(c);
    return c;
}

/* get a (possibly pushed back) character */
int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

/* push a character back on input */
void ungetch(int c)
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}

/* Note: Leading zeros are omitted, so that 00001234 is treated as 1234. Of
   course there can be other ways. */
