/* Exercise 4-11. Modify getop so that it doesn't need to use ungetch. Hint:
   use an internal static variable. */

/* Revised based on the simple version on textbook (Section 4.3, page 78) */

/* calc.h*/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>     /* for atof() */

#define MAXOP 100       /* max size of operand or operator */
#define NUMBER '0'      /* signal that a number was found */
#define MAXVAL 100      /* maximum depth of val stack */

int sp = 0;             /* next free stack position */
double val[MAXVAL];     /* value stack */

double mod(double, double);
int getop(char []);
void push(double);
double pop(void);

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
    static int lastc = '\0';

    if (lastc == '\0')
        c = getchar();
    else {
        c = lastc;
        lastc = '\0';
    }

    while ((s[0] = c) == ' ' || c == '\t')
        c = getchar();
    s[1] = '\0';
    if (!isdigit(c) && c != '.')
        return c;
    i = 0;
    if (isdigit(c))
        while (isdigit(s[++i] = c = getchar()))
            ;
    if (c == '.')
        while (isdigit(s[++i] = c = getchar()))
            ;
    s[i] = '\0';
    if (c != EOF)
        lastc = c;
    return NUMBER;
}

/* Note: This only works conveniently with one occurrence of ungetch()
   throughout the getop(). Otherwise, it can get really cumbersome. */
