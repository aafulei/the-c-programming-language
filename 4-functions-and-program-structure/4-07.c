/* Exercise 4-7. Write a routine ungets(s) that will push back an entire string
   onto the input. Should ungets know about buf and bufp, or should it just use
   ungetch? */

/* Answer: ungets() should just use ungetch without knowing about buf, bufp. */

#include <stdio.h>

#define MAXCHAR 10
#define BUFSIZE 100

char buf[BUFSIZE];  /* buffer for ungetch */
int bufp = 0;       /* next free position in buf */

void ungets(char []);
int getch(void);
void ungetch(int);

/* test: get a string of less than 10 characters, push the entire string back,
   and then re-get the characters one by one from the buffer */
int main(void)
{
    int c, i;
    char test[MAXCHAR];

    i = 0;
    for (i = 0; i < MAXCHAR-1 && (c = getch()) != EOF && c != '\n'; i++)
        test[i] = c;
    test[i] = '\0';
    printf("ungets(\"%s\") to buffer\n", test);
    ungets(test);
    while (i-- > 0)
        printf("re-get '%c' from buffer\n", getch());
    return 0;
}

/* push back an entire string */
void ungets(char s[])
{
    int i;

    for (i = 0; s[i] != '\0'; i++)
        ;
    while (i > 0)
        ungetch(s[--i]);
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
