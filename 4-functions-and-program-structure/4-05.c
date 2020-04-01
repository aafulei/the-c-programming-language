/* Exercise 4-5. Add access to library functions like sin, exp, and pow. See
   <math.h> in Appendix B, Section 4. */

/* See Section 4.3 (page 76-79) for the calculator framework. */

/* calc.h*/
#include <ctype.h>
#include <math.h>       /* for sin(), exp(), pow() */
#include <stdio.h>
#include <stdlib.h>     /* for atof() */
#include <string.h>     /* for strcmp() */

#define MAXOP 100       /* max size of operand or operator */
#define NUMBER '0'      /* signal that a number was found */
#define FUNC '1'        /* signal that a function was found */
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
void clear(void);
int getch(void);
void ungetch(int);

/* main.c */
/* reverse Polish calculator */
int main(void)
{
    int type;
    double op1, op2;
    char s[MAXOP];

    while ((type = getop(s)) != EOF) {
        switch (type) {
        case NUMBER:
            push(atof(s));
            break;
        /* function type */
        case FUNC:
            if (strcmp(s, "sin") == 0)
                push(sin(pop()));
            else if (strcmp(s, "cos") == 0)
                push(cos(pop()));
            else if (strcmp(s, "exp") == 0)
                push(exp(pop()));
            else if (strcmp(s, "log") == 0)
                push(log(pop()));
            else if (strcmp(s, "log10") == 0)
                push(log10(pop()));
            else if (strcmp(s, "pow") == 0) {
                op2 = pop();
                push(pow(pop(), op2));
            }
            else if (strcmp(s, "sqrt") == 0)
                push(sqrt(pop()));
            else
                printf("error: unknown function %s\n", s);
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
        case '%':
            op2 = pop();
            if (op2 != 0.0)
                push(mod(pop(), op2));
            else
                printf("error: zero divisor\n");
            break;
        case '?':
            op2 = pop();
            printf("\t%.8g\n", op2);
            push(op2);
            break;
        case 'd':
            op2 = pop();
            push(op2);
            push(op2);
            break;
        case 's':
            op2 = pop();
            op1 = pop();
            push(op2);
            push(op1);
            break;
        case 'c':
            clear();
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

void clear(void)
{
    sp = 0;
}

/* getop.c */
/* getop: get next character or numeric operand */
int getop(char s[])
{
    int i, c;

    while ((s[0] = c = getch()) == ' ' || c == '\t')
        ;
    s[1] = '\0';
    i = 0;
    if (!isdigit(c) && c != '.' && c != '-') {
        if (isalpha(c)) {
            while (isgraph(c = getch()))
                s[++i] = c;
            s[++i] = '\0';
            if (c != EOF)
                ungetch(c);
            /* single letter: command */
            if (i == 1)
                return s[0];
            /* multiple letters: function */
            else
                return FUNC;
        }
        else
            return c;
    }
    if (c == '-')
        if (isdigit(c = getch()) || c == '.')
            s[++i] = c;
        else {
            if (c != EOF)
                ungetch(c);
            return '-';
        }
    if (isdigit(c))
        while (isdigit(s[++i] = c = getch()))
            ;
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

/* Note: strcmp returns 0 if strings are equal. This means that
    if (strcmp(s1, s2))
        printf("s1 and s2 are NOT equal!\n"); */
