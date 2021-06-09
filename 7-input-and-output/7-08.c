/* Exercise 7-8. Write a program to print a set of files, starting each new one
 * on a new page, with a title and a running page count for each file. */

#include <stdio.h>
#define PAGELINES 20
#define MAXLINE 1000

int fgetline(FILE *fp, char *line, int lim)
{
  int c, n = 0;
  while ((c = getc(fp)) != EOF && c != '\n') {
    if (n++ < lim)
      *line++ = c;
  }
  *line = '\0';
  return c;
}

void display(FILE *fp, const char *fn, int pagelines)
{
  printf("===== %s =====\n\n", fn);
  int c;
  char line[MAXLINE];
  int pageno = 1;
  int lineno = 1;
  while (fgetline(fp, line, MAXLINE) != EOF) {
    printf("%4d: %s\n", lineno, line);
    if (lineno % pagelines == 0)
      printf("\n----- page %d -----\n\n", pageno++);
    ++lineno;
  }
  printf("\n\n\n\n");
}

int main(int argc, char *argv[])
{
  if (argc == 1) {
    fprintf(stderr, "usage: %s file1 [file2] [file3]\n", argv[0]);
    return 1;
  }
  for (int i = 1; i != argc; ++i) {
    FILE *fp;
    if ((fp = fopen(argv[i], "r")) == NULL) {
      fprintf(stderr, "cannot open %s\n", argv[i]);
      continue;
    }
    display(fp, argv[i], PAGELINES);
    fclose(fp);
  }
  return 0;
}
