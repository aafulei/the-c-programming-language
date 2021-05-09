/* Exercise 5-12. Extend entab and detab to accept the shorthand
entab -m +n to mean tab stops every n columns, starting at column m. Choose
convenient (for the user) default behavior. */

#include <stdio.h>

#define DEFAULT_TAB_BEGIN 0
#define DEFAULT_TAB_SIZE  8

static int g_tab_beg = DEFAULT_TAB_BEGIN;
static int g_tab_size = DEFAULT_TAB_SIZE;

static int g_next_stop = 0;


void reset_next_stop()
{
  g_next_stop = g_tab_beg;
}

void update_next_stop()
{
  g_next_stop += g_tab_size;
}

void detab()
{
  int c;
  int i = 0;
  while ((c = getchar()) != EOF) {
    if (c != '\t') {
      putchar(c);
      ++i;
      if (c == '\n') {
        i = 0;
        reset_next_stop();
      }
    }
    else {
      while (i != g_next_stop) {
        putchar(' ');
        ++i;
      }
    }
    if (i == g_next_stop) {
      update_next_stop();
    }
  }
}

int main(int argc, char *argv[])
{
  for (int i = 1; i != argc; ++i) {
    if (argv[i][0] == '-')
      sscanf(argv[i] + 1, "%d", &g_tab_beg);
    else if (argv[i][0] == '+')
      sscanf(argv[i] + 1, "%d", &g_tab_size);
    else
      fprintf(stderr, "ignore %s\n", argv[i]);
  }
  fprintf(stderr, "g_tab_beg = %d, g_tab_size = %d\n", g_tab_beg, g_tab_size);
  reset_next_stop();
  detab();
  return 0;
}
