/* Exercise 4-3. Given the basic framework, it's straightforward to extend the
   calculator. Add the modulus (%) operator and provisions for negative
   numbers. */

/* See Section 4.3 (page 76-79) for the calculator framework. */

/* calc.h*/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>     /* for atof() */

#define MAXOP 100       /* max size of operand or operator */
#define NUMBER '0'      /* signal that a number was found */
#define MAXVAL 100      /* maximum depth of val stack */
#define BUFSIZE 100

int sp = 0;             /* next free stack position */
double val[MAXVAL];     /* value stack */
char buf[BUFSIZE];      /* buffer for ungetch */
int bufp = 0;           /* next free position in buf */

double mod(double, double);
int getop(char []);
void push(double);
double pop(void);
int getch(void);
void ungetch(int);

/* main.c */
/* reverse Polish calculator */
int main(void)
{
    int type;
    double op2;
    char s[MAXOP];

    while ((type = getop(s)) != EOF) {
        switch (type) {
        case NUMBER:
            push(atof(s));
            break;
        case '+':
            push(pop() + pop());
            break;
        case '*':
            push(pop() * pop());
            break;
        case '-':
            op2 = pop();
            push(pop() - op2);
            break;
        case '/':
            op2 = pop();
            if (op2 != 0.0)
                push(pop() / op2);
            else
                printf("error: zero divisor\n");
            break;
        /* modulus */
        case '%':
            op2 = pop();
            if (op2 != 0.0)
                push(mod(pop(), op2));
            else
                printf("error: zero divisor\n");
            break;
        case '\n':
            printf("\t%.8g\n", pop());
            break;
        default:
            printf("error: unknown command %s\n", s);
            break;
        }
    }
    return 0;
}

double mod(double a, double b)
{
    return a - (int)a / (int)b * b;
}

/* stack.c */
/* push:  push f onto value stack */
void push(double f)
{
   if (sp < MAXVAL)
       val[sp++] = f;
   else
       printf("error: stack full, can't push %g\n", f);
}

/* pop: pop and return top value from stack */
double pop(void)
{
    if (sp > 0)
        return val[--sp];
    else {
        printf("error: stack empty\n");
        return 0.0;
    }
}

/* getop.c */
/* getop: get next character or numeric operand */
int getop(char s[])
{
    int i, c;

    while ((s[0] = c = getch()) == ' ' || c == '\t')
        ;
    s[1] = '\0';
     /* not a number */
    if (!isdigit(c) && c != '.' && c != '-')
        return c;
    i = 0;
    if (c == '-')
        /* negative number */
        if (isdigit(c = getch()) || c == '.')
            s[++i] = c;
         /* minus sign */
        else {
            if (c != EOF)
                ungetch(c);
            return '-';
        }
    /* integral part */
    if (isdigit(c))
        while (isdigit(s[++i] = c = getch()))
            ;
    /* fractional part */
    if (c == '.')
        while (isdigit(s[++i] = c = getch()))
            ;
    s[i] = '\0';
    if (c != EOF)
        ungetch(c);
    return NUMBER;
}

/* getch.c */
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
