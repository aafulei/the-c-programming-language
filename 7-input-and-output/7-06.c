/* Exercise 7-6. Write a program to compare two files, printing the first line
 * where they differ. */

#include <stdio.h>
#include <string.h>

int fgetline(FILE *fp, char *line, int lim)
{
  int c, n = 0;
  while ((c = getc(fp)) != EOF && c != '\n') {
    if (n++ < lim) {
      *line++ = c;
    }
  }
  *line = '\0';
  return c;
}

int main(int argc, char *argv[])
{
  if (argc != 3) {
    fprintf(stderr, "usage: %s file1 file2\n", argv[0]);
    return 1;
  }
  FILE *fp1, *fp2;
  if ((fp1 = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "cannot open %s\n", argv[1]);
    return 1;
  }
  if ((fp2 = fopen(argv[2], "r")) == NULL) {
    fprintf(stderr, "cannot open %s\n", argv[2]);
    return 1;
  }

  char line1[1000], line2[1000];
  int res1, res2, lineno = 0;
  while (1) {
    ++lineno;
    res1 = fgetline(fp1, line1, 1000) == EOF;
    res2 = fgetline(fp2, line2, 1000) == EOF;
    switch (res1 + 2 * res2) {
      case 0:
        if (strcmp(line1, line2) != 0) {
          printf("%s:%d: %s\n", argv[1], lineno, line1);
          printf("%s:%d: %s\n", argv[2], lineno, line2);
          return 0;
        }
        else
          continue;
      case 1:
        printf("%s:%d: %s\n", argv[2], lineno, line2);
        return 0;
      case 2:
        printf("%s:%d: %s\n", argv[1], lineno, line1);
        return 0;
      case 3:
        return 0;
    }
  }
  return 0;
}
