/* Exercise 4-10. An alternate organization uses getline to read an entire
   input line; this makes getch and ungetch unnecessary. Revise the calculator
   to use this approach. */

/* Revised based on code for Exercise 4-06. */

/* calc.h*/
#include <ctype.h>
#include <math.h>       /* for sin(), exp(), pow() */
#include <stdio.h>
#include <stdlib.h>     /* for atof() */
#include <string.h>     /* for strcmp() */

#define MAXOP 100       /* max size of operand or operator */
#define NA -1           /* not applicable (for variable pointer) */
#define MAXVAL 100      /* maximum depth of val stack */
#define MAXLINE 1000

int sp = 0;             /* next free stack position */
double val[MAXVAL];     /* value stack */

double mod(double, double);
void push(double);
double pop(void);
void clear(void);
int getline(char [], int);

/* main.c */
/* reverse Polish calculator */
int main(void)
{
    double op1, op2, last;
    double var[26];         /* variables from A to Z */
    int vp;                 /* variable pointer */
    int typevar;            /* current op is a variable (not number, etc.) */
    char s[MAXOP];
    char line[MAXLINE];
    int len, i, j;

    last = 0.0;
    for (vp = 0; vp < 26; vp++)
        var[vp] = 0.0;
    vp = NA;
    while ((len = getline(line, MAXLINE)) > 0) {
        for (i = 0; i < len; ) {
            while (line[i] == ' ' || line[i] == '\t')
                i++;
            typevar = 0;
            /* number */
            if (isdigit(line[i]) || line[i] == '.' ||
                line[i] == '-' && (isdigit(line[i+1]) || line[i+1] == '.')) {
                j = 0;
                if (line[i] == '-')
                    s[j++] = line[i++];
                while (isdigit(line[i]))
                    s[j++] = line[i++];
                if (line[i] == '.')
                    s[j++] = line[i++];
                while (isdigit(line[i]))
                    s[j++] = line[i++];
                s[j] = '\0';
                push(atof(s));
            }
            /* function */
            else if (isalpha(line[i]) && isgraph(line[i+1])) {
                j = 0;
                while (isgraph(line[i]))
                    s[j++] = line[i++];
                s[j] = '\0';
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
            }
            /* variable */
            else if ('A' <= line[i] && line[i] <= 'Z') {
                vp = line[i++] - 'A';
                push(var[vp]);
                typevar = 1;
            }
            /* operator */
            else
                switch(line[i++]) {
                case '=':
                    if (vp == NA)
                        printf("error: cannot assign\n");
                    else {
                        pop();
                        push(var[vp] = pop());
                    }
                    break;
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
                default:
                    printf("error: unknown command %s\n", s);
                    break;
                }
            if (!typevar)
                vp = NA;
        }
        printf("\t%.8g\n", last = pop());
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

/* getline.c */
/* easy getline: read a line, stop at max len or EOF or newline, return len */
int getline(char s[], int lim)
{
    int c, i;

    for (i = 0; i < lim-1 && (c=getchar()) != EOF && c != '\n'; i++)
        s[i] = c;
    s[i] = '\0';
    return i;
}

/* Note: No need to worry about out-of-bounds array access for line[i+1] as
   i<len and there is an additional '\0' at line[len]. */
