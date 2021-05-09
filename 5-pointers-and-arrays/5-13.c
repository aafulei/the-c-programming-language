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
  return (c == EOF) ? EOF : i;
}

int main(int argc, char *argv[])
{
  int num = DEFAULT_LINES;
  if (argc == 2 && sscanf(argv[1], "-%d", &num) != 1 || argc > 2) {
    fprintf(stderr, "usage: tail -n\n");
    return 1;
  }
  if (num == 0 || num > MAX_LINES) {
    num = MAX_LINES;
    fprintf(stderr, "warning: n out of range, set to max value %d\n", num);
  }
  char **lines = malloc(num * sizeof(char *));
  if (!lines) {
    fprintf(stderr, "malloc() on lines failed!\n");
    return 1;
  }
  const size_t N = num * (MAX_LEN + 1);
  char *buf = malloc(N);
  if (!buf) {
    fprintf(stderr, "malloc() on buf failed!\n");
    return 1;
  }
  int i = 0;
  int j = 0;
  int n = 0;
  char line[MAX_LEN + 1];
  for (int len; (len = getline_(line, MAX_LEN)) != EOF; ++n) {
    strcpy(buf + j, line);
    lines[i] = buf + j;
    j += len + 1;
    j *= (j < N);
    i = (i + 1) % num;
  }
  if (n > num)
    n = num;
  i = (i + num - n) % num;
  while (n-- > 0) {
    printf("%s\n", lines[i]);
    i = (i + 1) % num;
  }
  return 0;
}
