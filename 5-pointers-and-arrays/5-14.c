/* Exercise 5-14. Modify the sort porogram to handle a -r flag, which indicates
 * sorting in reverse (decreasing) order. Be sure that -r works with -n. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN   200
#define MAXLINES 5000
#define BUFSIZE  10000
#define NUMERIC  1
#define REVERSE  2

static char g_buffer[BUFSIZE];
static char *g_next = g_buffer;

char *lineptr[MAXLINES];

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);

void qsort_(void *lineptr[], int left, int right, int (*comp)(void *, void *));
int numcmp(const char *, const char *);
int rnumcmp(const char *s, const char *t);
int rstrcmp(const char *s, const char *t);

int main(int argc, char *argv[])
{
  int nlines;
  int option = 0;
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-')
      for (int j = 1; argv[i][j] != '\0'; ++j) {
        switch (argv[i][j]) {
          case 'n':
            option |= NUMERIC;
            break;
          case 'r':
            option |= REVERSE;
            break;
          default:
            fprintf(stderr, "unknown option %c\n", argv[i][j]);
            break;
        }
      }
  }
  fprintf(stderr, "numeric = %d, reverse = %d\n", option & NUMERIC,
          option & REVERSE);
  if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
    int (*fp)(const char *, const char *);
    if (option & NUMERIC) {
      if (option & REVERSE)
        fp = rnumcmp;
      else
        fp = numcmp;
    }
    else {
      if (option & REVERSE)
        fp = rstrcmp;
      else
        fp = strcmp;
    }
    qsort_((void **)lineptr, 0, nlines - 1, (int (*)(void *, void *))fp);
    writelines(lineptr, nlines);
    return 0;
  }
  else {
    printf("intput too big to sort\n");
    return 1;
  }
}

int (*rev(int (*f)(void *, void *), int r))(void *, void *) {}

void qsort_(void *v[], int left, int right, int (*comp)(void *, void *))
{
  int i, last;
  void swap(void *v[], int, int);
  if (left >= right)
    return;
  swap(v, left, (left + right) / 2);
  last = left;
  for (i = left + 1; i <= right; ++i)
    if ((*comp)(v[i], v[left]) < 0)
      swap(v, ++last, i);
  swap(v, left, last);
  qsort_(v, left, last - 1, comp);
  qsort_(v, last + 1, right, comp);
}

void swap(void *v[], int i, int j)
{
  int *t;
  t = v[i];
  v[i] = v[j];
  v[j] = t;
}

int numcmp(const char *s, const char *t)
{
  double a = atof(s);
  double b = atof(t);
  return (a > b) - (a < b);
}

int rnumcmp(const char *s, const char *t)
{
  return numcmp(t, s);
}

int rstrcmp(const char *s, const char *t)
{
  return strcmp(t, s);
}

void *alloc(int nb)
{
  if (g_next + nb <= g_buffer + BUFSIZE) {
    void *res = g_next;
    g_next += nb;
    return res;
  }
  else
    return NULL;
}

void afree(void *p)
{
  if (!p)
    return;
  if (g_buffer <= (char *)p && (char *)p < g_next)
    g_next = p;
}

int getline_(char s[], int lim)
{
  int c, i;
  i = 0;
  while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
    s[i++] = c;
  if (c == '\n')
    s[i++] = c;
  s[i] = '\0';
  return i;
}

int readlines(char *lineptr[], int maxlines)
{
  int len, nlines;
  char *p, line[MAXLEN];
  nlines = 0;
  while ((len = getline_(line, MAXLEN)) > 0)
    if (nlines >= maxlines || (p = alloc(len)) == NULL)
      return -1;
    else {
      line[len - 1] = '\0';
      strcpy(p, line);
      lineptr[nlines++] = p;
    }
  return nlines;
}

void writelines(char *lineptr[], int nlines)
{
  int i;
  for (i = 0; i < nlines; ++i)
    printf("%s\n", lineptr[i]);
}
