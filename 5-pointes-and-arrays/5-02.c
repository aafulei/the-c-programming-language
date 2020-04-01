/* Exercise 5-2. Write getfloat, the floating-point analog of getint. What type
   does getfloat return as its function value? */

/* Answer: getfloat returns an integer:
            (1) 1 if input contains a valid number,
            (2) 0 if next input is not a number,
            (3) EOF for end of file. */

#include <ctype.h>
#include <stdio.h>
#include <math.h>

#define BUFSIZE 100

static int buf[BUFSIZE];
static int bufp = 0;

int getfloat(double *);
int getch(void);
void ungetch(int);
int peek1(void);
int peek2(void);

/* test: try to get a floating-point number, print result and next char */
int main(void)
{
    double n;
    int c;

    switch(getfloat(&n)) {
        case 0: printf("not a number\n"); break;
        case EOF: printf("end of file\n"); return 0;
        default: printf("%f\n", n);
    }
    printf("next char: ");
    c = getch();
    printf(isgraph(c) ? "%c\n" : "[%d]\n", c);
    return 0;
}

/* get next floating-point number from input into *pn */
int getfloat(double *pn)
{
    int c, sign, isvalid, expsign;
    double power;

    while (isspace(c=getch()))
        ;
    if (c == EOF)
        return EOF;
    if (!isdigit(c) && c != '+' && c != '-' && c != '.') {
        ungetch(c);
        return 0;
    }
    sign = c == '-' ? -1 : 1;
    isvalid = 0;
    if ((c == '+' || c == '-') && !isdigit(c=getch())) {
        if (c == '.') {
            if (isdigit(c=getch())) {
                ungetch(c);
                c = '.';
                isvalid = 1;
            }
            else {
                if (c != EOF)
                    ungetch(c);
                ungetch('.');
            }
        }
        else {
            if (c != EOF)
                ungetch(c);
        }
        if (!isvalid) {
            ungetch(sign == 1 ? '+' : '-');
            return 0;
        }
    }
    for (*pn = 0; isdigit(c); c = getch())
        *pn = *pn * 10 + (c-'0');
    if (c == '.')
        c = getch();
    for (power = 1; isdigit(c); c = getch())
        *pn += (c-'0') / (power *= 10);
    if (tolower(c) == 'e' &&
        (isdigit(expsign=peek1()) ||
         ((expsign == '+'|| expsign == '-') && isdigit(peek2())))) {
        c = getch();
        expsign = c == '-' ? -1 : 1;
        if (c == '+' || c == '-')
            c = getch();
        for (power = 0; isdigit(c); c = getch())
            power = power * 10 + (c-'0');
        *pn *= pow(10, expsign * power);
    }
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

/* peek the next character on input */
int peek1(void)
{
    int c;

    c = getch();
    ungetch(c);
    return c;
}

/* peek the second next character on input */
int peek2(void)
{
    int c1, c2;

    c1 = getch();
    c2 = getch();
    ungetch(c2);
    ungetch(c1);
    return c2;
}

/* Note: A valid floating-point number has the form
    [+/-]{integer}[.]{fraction}[e/E][+/-]{exponent}
   where (1) all parts optional, but {integer} and {fraction} must have one;
         (2) a dangling [e/E][+/-] such as the "e+" in string "123.45e+" is not
             considered part of number, and thus the next character after
             reading a floating-point number (123.45) is "e". */
