/* Exercise 4-6. Add commands for handling variables. (It's easy to provide
   twenty-six variables with single-letter names.) Add a variable for the most
   recently printed value. */

/* calc.h*/
#include <ctype.h>
#include <math.h>       /* for sin(), exp(), pow() */
#include <stdio.h>
#include <stdlib.h>     /* for atof() */
#include <string.h>     /* for strcmp() */

#define MAXOP 100       /* max size of operand or operator */
#define NUMBER '0'      /* signal that a number was found */
#define FUNC '1'        /* signal that a function was found */
#define VAR '2'         /* signal that a variable was found */
#define NA -1           /* not applicable (for variable pointer) */
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
    double op1, op2, last;
    double var[26];         /* variables from A to Z */
    int vp;                 /* variable pointer */
    char s[MAXOP];

    last = 0.0;
    for (vp = 0; vp < 26; vp++)
        var[vp] = 0.0;
    vp = NA;
    while ((type = getop(s)) != EOF) {
        switch (type) {
        case NUMBER:
            push(atof(s));
            break;
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
        case VAR:
            push(var[vp = s[0]-'A']);
            break;
        /* assign to variable */
        case '=':
            if (vp == NA)
                printf("error: cannot assign\n");
            else {
                pop();
                push(var[vp] = pop());
            }
            break;
        /* last printed value */
        case '_':
            push(last);
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
            /* save printed value */
            printf("\t%.8g\n", last = pop());
            break;
        default:
            printf("error: unknown command %s\n", s);
            break;
        }
         /* clear variable pointer */
        if (type != VAR)
            vp = NA;
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
            if (i == 1)
                 /* variable */
                if ('A' <= s[0] && s[0] <= 'Z')
                    return VAR;
                else
                    return s[0];
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

/* Note: Variable names range from A to Z. Use _ for last printed value.
   Use operator = to assign a value to a variable. This is the only way to
   change a variable's value. All other operators such as + use a variable's
   value without changing it. All variables are initialized to 0. */
