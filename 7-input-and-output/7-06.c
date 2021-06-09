/* Exercise 7-6. Write a program to compare two files, printing the first line
 * where they differ. */

#include <stdio.h>
#include <string.h>

#define MAXLINE 1000

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
  char line1[MAXLINE], line2[MAXLINE];
  int res1, res2, lineno = 0;
  do {
    ++lineno;
    res1 = fgetline(fp1, line1, MAXLINE) != EOF;
    res2 = fgetline(fp2, line2, MAXLINE) != EOF;
  }
  while (res1 && res2 && strcmp(line1, line2) == 0);
  if (res1)
    printf("%s:%d: %s\n", argv[1], lineno, line1);
  if (res2)
    printf("%s:%d: %s\n", argv[2], lineno, line2);
  fclose(fp1);
  fclose(fp2);
  return 0;
}
