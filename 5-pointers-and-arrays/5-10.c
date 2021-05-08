/* Exercise 5-10. Write the program expr, which evaluates a reverse Polish
expression from the command line, where each operator or operand is a separate
argument. For example,

  expr 2 3 4 + *

evaluates 2 * (3 + 4). */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define BUSIZE 100

static float g_num_stk[BUSIZE];
static float *g_num_next = g_num_stk;

int main(int argc, char *argv[])
{
  for (int i = 1; i < argc; ++i) {
    char c = argv[i][0];
    if (isdigit(c) || c == '.') {
      if (g_num_next == g_num_stk + BUSIZE) {
        printf("try to push to a full stack!\n");
        return EXIT_FAILURE;
      }
      if (sscanf(argv[i], "%f", g_num_next++) != 1) {
        printf(". is neither a number nor an operator!\n");
        return EXIT_FAILURE;
      }
    }
    else {
      if (g_num_next == g_num_stk) {
        printf("try to pop from an empty stack!\n");
        return EXIT_FAILURE;
      }
      float rhs = *--g_num_next;
      if (g_num_next == g_num_stk) {
        printf("try to pop from an empty stack!\n");
        return EXIT_FAILURE;
      }
      --g_num_next;
      switch (c) {
        case '+':
          *g_num_next++ += rhs;
          break;
        case '-':
          *g_num_next++ -= rhs;
          break;
        case '*':
          *g_num_next++ *= rhs;
          break;
        case '/':
          *g_num_next++ /= rhs;
          break;
        default:
          printf("unknown operator!\n");
          return EXIT_FAILURE;
      }
    }
  }
  if (g_num_next == g_num_stk) {
    printf("try to pop from an empty stack!\n");
    return EXIT_FAILURE;
  }
  printf("%g\n", *--g_num_next);
  return EXIT_SUCCESS;
}
