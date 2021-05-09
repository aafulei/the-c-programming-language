/* Exercise 5-11. Modify the program entab and detab (written as exercises in
Chapter 1) to accept a list of tab stops as arguments. Use the default tab
settings if there are no arguments. */

/* Exercise 1-20. Write a program detab that replaces tabs in the input with
   the proper number of blanks to space to the next tab stop. Assume a fixed
   set of tab stops, say every n columns. Should n be a variable or a symbolic
   parameter? */

// detab 4 8 20
// abc\tdef\tghij\tklmn\topq
// abc def ghij        klmn        opq

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABSIZE 8
#define MAXSIZE 100

static int g_tab_stop[MAXSIZE];
static int g_N = 0;
static int g_i = 0;
static int g_next_stop = 0;

void flush(int, int);

void debug()
{
  printf("\n[g_tab_stop]\n");
  for (int i = 0; i != g_N; ++i)
    printf("%d ", g_tab_stop[i]);
  printf("\n");
}

int find_num_spaces(int nc)
{
  while (g_i < g_N && g_tab_stop[g_i] <= nc) {
    ++g_i;
  }
  if (g_i < g_N)
    return g_tab_stop[g_i] - nc;
  else
    return TABSIZE - nc % TABSIZE;
}

void reset_next_stop()
{
  if (g_N) {
    g_i = 0;
    g_next_stop = g_tab_stop[g_i];
  }
  else
    g_next_stop = TABSIZE;
}

void update_next_stop()
{
  ++g_i;
  if (g_i < g_N)
    g_next_stop = g_tab_stop[g_i];
  else if (g_N && g_next_stop == g_tab_stop[g_N - 1])
    g_next_stop += (TABSIZE - g_next_stop % TABSIZE);
  else
    g_next_stop += TABSIZE;
}

void entab()
{
  int c;
  int i = 0;
  int ns = 0;
  while ((c = getchar()) != EOF) {
    if (c != ' ' && c != '\t') {
      for (; ns; --ns)
        putchar(' ');
      putchar(c);
      ++i;
      if (c == '\n') {
        i = 0;
        reset_next_stop();
      }
    }
    else if (c == ' ') {
      ++ns;
      ++i;
    }
    else {
      putchar('\t');
      ns = 0;
      i = g_next_stop;
    }
    if (i == g_next_stop) {
      if (ns) {
        putchar('\t');
        ns = 0;
      }
      update_next_stop();
    }
  }
}

int cmp(const void *a, const void *b)
{
  int ia = *(const int *)a;
  int ib = *(const int *)b;
  return (ia > ib) - (ia < ib);
}

int main(int argc, char *argv[])
{
  int c;
  int to_entab = 0;
  for (int i = 0, j = 0; i < argc - 1 && i < MAXSIZE; ++i) {
    if (argv[i + 1][0] == '-') {
      if (strcmp(argv[i + 1], "--entab") == 0)
        to_entab = 1;
      else if (strcmp(argv[i + 1], "--detab") == 0)
        to_entab = 0;
      else {
        printf("warning: unknown option %s will be ignored", argv[i + 1]);
      }
    }
    else {
      sscanf(argv[i + 1], "%d", &g_tab_stop[g_N++]);
    }
  }
  qsort(g_tab_stop, g_N, sizeof(int), cmp);
  reset_next_stop();
  // debug();
  fprintf(stderr, "g_next_stop = %d\n", g_next_stop);
  entab();
  return 0;
}
