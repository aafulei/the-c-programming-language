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

/* entab: replace blanks in input with minimum number of tabs and blanks */
int entab()
{
  int c, nc, ns;
  nc = 0;
  ns = 0;
  while ((c = getchar()) != EOF) {
    if (c == ' ')
      ++ns;
    else if (c == '\t') {
      int old_i = g_i;
      ns = find_num_spaces(nc + ns);
      g_i = old_i;
    }
    else {
      flush(nc, ns);
      nc = nc + ns;
      ns = 0;
      putchar(c);
      if (c == '\n') {
        nc = 0;
        ns = 0;
        g_i = 0;
      }
      else
        ++nc;
    }
  }
  flush(nc, ns);
  return 0;
}

/* given i characters in current line, flush j blanks out of buffer */
void flush(int nc, int ns)
{
  if (ns == 0)
    return;
  if (nc < g_tab_stop[g_N - 1]) {
    int nt = 0;
    while (g_i < g_N && g_tab_stop[g_i] <= nc)
      ++g_i;
    while (g_i < g_N && g_tab_stop[g_i] <= nc + ns) {
      ++g_i;
      ++nt;
    }
    if (g_i < g_N) {
      if (g_i > 0) {
        ns = nc + ns - g_tab_stop[g_i - 1];
      }
      while (nt-- > 0)
        putchar('\t');
      while (ns-- > 0)
        putchar(' ');
    }
    else {
      while (nt-- > 0)
        putchar('\t');
      ns = nc + ns - g_tab_stop[g_N - 1];
      nc = g_tab_stop[g_N - 1];
    }
  }
  if (nc >= g_tab_stop[g_N - 1]) {
    int nx = TABSIZE - nc % TABSIZE;
    while (ns >= nx) {
      putchar('\t');
      ns -= nx;
      nx = TABSIZE;
    }
    while (ns-- > 0) {
      putchar(' ');
    }
  }
}

/* detab: replace tabs in input with proper number of blanks */
int detab()
{
  int c, nc, ns;
  nc = 0;
  while ((c = getchar()) != EOF) {
    if (c == '\t') {
      ns = find_num_spaces(nc);
      nc = nc + ns;
      while (ns-- > 0)
        putchar(' ');
    }
    else {
      putchar(c);
      if (c == '\n') {
        nc = 0;
        g_i = 0;
      }
      else
        ++nc;
    }
  }
  return 0;
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
  if (to_entab)
    entab();
  else
    detab();
  return 0;
}
