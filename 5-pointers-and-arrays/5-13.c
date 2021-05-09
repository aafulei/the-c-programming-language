/* Exercise 5-13. Write the program tail, which prints the last n lines of its
input. By default, n is set to 10, let us say, but it can be changed by an
optional argument so that tail -n prints the last n lines. The program should
behave rationally no matter how unreasonable the input or the value of n. Write
the program so it makes the best use of available storage; lines should be
stored as in the sorting program of Section 5.6, not in a two-dimensional array
of fixed size. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE       10000
#define MAX_LEN       100
#define MAX_LINES     50
#define DEFAULT_LINES 10

static int g_lines = DEFAULT_LINES;

int getline_(char *line, int lim)
{
  int c;
  int i = 0;
  while ((c = getchar()) != EOF && c != '\n') {
    if (i < lim)
      *line++ = c;
    ++i;
  }
  *line = '\0';
  if (c == EOF)
    return EOF;
  else
    return i;
}

int main(int argc, char *argv[])
{
  if (argc == 2 && sscanf(argv[1], "-%d", &g_lines) != 1 || argc > 2) {
    fprintf(stderr, "usage: tail -n\n");
    return 1;
  }
  if (g_lines > MAX_LINES) {
    g_lines = MAX_LINES;
    fprintf(stderr, "warning: n too large, set to max possible at %d\n",
            g_lines);
  }
  char **lines = malloc(g_lines * sizeof(char *));
  if (!lines) {
    fprintf(stderr, "malloc() on lines failed!\n");
    return 1;
  }

  char **lineptr = lines;
  char line[MAX_LEN + 1];
  int len;
  char *buffer = malloc(g_lines * (MAX_LEN + 1));
  if (!buffer) {
    fprintf(stderr, "malloc() on buffer failed!\n");
    return 1;
  }
  char *next = buffer;
  int n = 0;
  while ((len = getline_(line, MAX_LEN)) != EOF) {
    if (n < g_lines)
      ++n;
    strcpy(next, line);
    *lineptr = next;
    if (next + len + 1 > buffer + g_lines * (MAX_LEN + 1))
      next = buffer;
    else
      next += len + 1;
    ++lineptr;
    if (lineptr == lines + g_lines)
      lineptr = lines;
  }

  if (lineptr < lines + n)
    lineptr = lineptr + (g_lines - n);
  else
    lineptr -= n;

  while (n-- > 0) {
    printf("%s\n", *lineptr);
    ++lineptr;
    if (lineptr == lines + g_lines)
      lineptr = lines;
  }
  return 0;
}
