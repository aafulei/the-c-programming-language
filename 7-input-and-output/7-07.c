/* Exercise 7-7. Modify the pattern finding program of Chapter 5 to take its
 * input from a set of named files or, if no files are named as arguments, from
 * the standard input. Should the file name be printed when a matching line is
 * found? */

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
  if (argc == 1) {
    fprintf(stderr, "usage: %s pattern [file1] [file2] ...\n", argv[0]);
    return 0;
  }
  char line[MAXLINE];
  int found = 0;
  if (argc == 2) {
    int n = 1;
    while (fgetline(stdin, line, MAXLINE) != EOF) {
      if (strstr(line, argv[1]) != NULL) {
        printf("stdin:%d:\t%s\n", n, line);
        ++found;
      }
      ++n;
    }
  }
  else {
    for (int i = 2; i < argc; ++i) {
      FILE *fp;
      if ((fp = fopen(argv[i], "r")) == NULL) {
        fprintf(stderr, "cannot open %s\n", argv[i]);
        continue;
      }
      int n = 1;
      while (fgetline(fp, line, MAXLINE) != EOF) {
        if (strstr(line, argv[1]) != NULL) {
          printf("%s:%d:\t%s\n", argv[i], n, line);
          ++found;
        }
        ++n;
      }
      fclose(fp);
    }
  }
  printf("-- found %d \"%s\" occurrence(s) --\n", found, argv[1]);
  return found;
}
