/* Exercise 7-5. Rewrite the postfix calculator of Chapter 4 to use scanf
 * and/or sscanf to do the input and number conversion. */

// modified from Exercise 4-3

/* calc.h*/
#include <ctype.h>
#include <stdio.h>
#include <assert.h>

#define MAXOP   100 /* max size of operand or operator */
#define NUMBER  '0' /* signal that a number was found */
#define MAXVAL  100 /* maximum depth of val stack */
#define BUFSIZE 100

int sp = 0;         /* next free stack position */
double val[MAXVAL]; /* value stack */
char buf[BUFSIZE];  /* buffer for ungetch */
int bufp = 0;       /* next free position in buf */

double mod(double, double);
int getop(char[]);
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

  double op;

  while ((type = getop(s)) != EOF) {
    switch (type) {
      case NUMBER:
        /* CHANGE */
        sscanf(s, "%lf", &op);
        push(op);
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

// 1. ignore whitespaces
// 2. if next is + - * / return c
// 3. if next is a number, fill s with string version of number, return NUMBER
int cpeek( FILE* fp)
{
    int ca = getc(fp);
    ungetc(ca,fp);
    return ca;
}

// CHANGE
int getop(char s[])
{
  int c;
  if ((c = cpeek(stdin)) == '\n' || c == EOF) {
    getchar();
    return c;
  }
  double num;
  int scanf_res = scanf("%s", s);
  assert(scanf_res);
  // printf("got s = %s\n", s);
  int sscanf_res = sscanf(s, "%lf", &num);
  // printf("(%d) got num = %lf\n", sscanf_res, num);
  if (sscanf_res) {
    return NUMBER;
  }
  else {
    return s[0];
  }
}
