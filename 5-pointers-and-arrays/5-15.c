/* Exercise 5-15. Add the option -f to fold upper and lower case together, so
 * that case distinctions are not made during sorting; for example, a and A
 * compare equal. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN   200
#define MAXLINES 5000
#define BUFSIZE  10000
#define NUMERIC  1
#define REVERSE  2
#define FOLD     4

static char g_buffer[BUFSIZE];
static char *g_next = g_buffer;
static char *lineptr[MAXLINES];

static int g_options = 0;

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);

void qsort_(void *lineptr[], int left, int right, int (*comp)(void *, void *));
int numcmp(const char *, const char *);
int rstrcmp(const char *, const char *);
int rchacmp(const char *, const char *);

int main(int argc, char *argv[])
{
  int nlines;
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-')
      for (int j = 1; argv[i][j] != '\0'; ++j) {
        switch (argv[i][j]) {
          case 'n':
            g_options |= NUMERIC;
            break;
          case 'r':
            g_options |= REVERSE;
            break;
          case 'f':
            g_options |= FOLD;
            break;
          default:
            fprintf(stderr, "unknown option %c\n", argv[i][j]);
            break;
        }
      }
  }
  fprintf(stderr, "numeric = %d, reverse = %d, fold = %d\n",
          g_options & NUMERIC, g_options & REVERSE, g_options & FOLD);
  if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
    int (*fp)(const char *, const char *);
    if (g_options & NUMERIC)
      fp = numcmp;
    else if (g_options & FOLD)
      fp = rchacmp;
    else
      fp = rstrcmp;
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
  int res = (a > b) - (a < b);
  if (g_options & REVERSE)
    return -res;
  else
    return res;
}

int rstrcmp(const char *s, const char *t)
{
  if (g_options & REVERSE)
    return strcmp(t, s);
  else
    return strcmp(s, t);
}

int tolower_(unsigned char c)
{
  if ('A' <= c && c <= 'Z')
    return c - 'A' + 'a';
  else
    return c;
}

int rchacmp(const char *s, const char *t)
{
  unsigned char a, b;
  int res;
  while (1) {
    a = tolower_(*(const unsigned char *)s++);
    b = tolower_(*(const unsigned char *)t++);
    if (a != b || a == '\0') {
      res = a - b;
      break;
    }
  }
  if (g_options & REVERSE)
    return -res;
  else
    return res;
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
