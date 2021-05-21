// to finish: use 2-d array, read a line into multiple fields

/* Exercise 5-17. Add a field-searching capability, so sorting may be done on
 * fields within lines, each field sorted according to an independent set of
 * options. (The index for this book was sorted with -df for the index category
 * and -n for the page numbers.) */

// Allen     Black       M   19    37280293
// Bruce     He          M   13    83940234
// Charlie   Wright      M   11    45720823
// Lisa      Wong        F    6    83092482
// Allen     Borton      M   17    80204030
// Alice     Wong        F    6    57102934
// Lucy      Black       F   14    40203942

// ./a.out -5
// ./a.out -5:n
// ./a.out -2,1 -4:nr

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXFIELDS 100
#define FIELDSIZE 100
#define MAXLEN    200
#define MAXLINES  5000
#define BUFSIZE   10000
// default opt is 0
#define NUMERIC   1
#define REVERSE   2
#define FOLD      4
#define DIR       8

static char g_buffer[BUFSIZE];
static char *g_next = g_buffer;
static char *lineptr[MAXLINES];

// static char *mat[MAXLINES][MAXFIELDS];
static char **mat[MAXLINES];

static int g_options = 0;

// g_opt[0] = 0
// g_opt[1] = NUMERIC & REVERSE
int g_opt[MAXFIELDS];

// g_col[0] = 2
// g_col[1] = 1
int g_col[MAXFIELDS];
int g_k = 0;
int g_nf = 1;
int g_maxrf = 0;

int readlines(void);
void writelines(char *lineptr[], int nlines);

void qsort_(void *lineptr[], int left, int right, int (*comp)(void *, void *));

int gencmp(const char **, const char **);

int numcmp(const char *, const char *);
int rstrcmp(const char *, const char *);
int rchacmp(const char *, const char *);

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

int field_cmp(const char **s, const char **t, int i)
{
  int (*fp)(const char *, const char *);
  // set global options
  g_options = g_opt[i];
  if (g_options & NUMERIC)
    fp = numcmp;
  else if (g_options & FOLD)
    fp = rchacmp;
  else
    fp = rstrcmp;
  return fp(s[g_col[i]-1], t[g_col[i]-1]);
}


int gencmp(const char **s, const char **t)
{
  int n = g_k;
  for (int i = 0, res; i != n; ++i) {
    if (g_col[i] - 1 > g_maxrf) {
      continue;
    }
    if ((res = field_cmp(s, t, i)) != 0)
      return res;
  }
  return 0;
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
  if (g_options & DIR) {
    while (!isalnum(*s) && *s != ' ' && *s != '\0')
      ++s;
    while (!isalnum(*t) && *t != ' ' && *t != '\0')
      ++t;
  }
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
  if (g_options & DIR) {
    while (!isalnum(*s) && *s != ' ' && *s != '\0')
      ++s;
    while (!isalnum(*t) && *t != ' ' && *t != '\0')
      ++t;
  }
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

int readlines(void)
{
  int len, nlines;
  char *p, line[MAXLEN];
  nlines = 0;
  while ((len = getline_(line, MAXLEN)) > 0) {
    if (nlines >= MAXLINES || (p = malloc(len)) == NULL)
      return -1;
    else {
      line[len - 1] = '\0';
      strcpy(p, line);
      mat[nlines] = calloc(MAXFIELDS, sizeof(char *));
      int k= 0;
      for (int j = 0, prevj = 0, ready = 0; j != len; ++j) {
          if (line[j] == '\t')
            ready = 1;
          else if (ready) {
            ready = 0;
            char *q = malloc(j - prevj + 1);
            strncpy(q, line + prevj, j - prevj);
            mat[nlines][k] = q;
            // printf("mat[%d][%d] = %s\n", nlines, k, q);
            prevj = j;
            ++k;
          }
          else if (line[j] == '\0') {
            char *q = malloc(j - prevj + 1);
            strncpy(q, line + prevj, j - prevj);
            mat[nlines][k] = q;
            // printf("mat[%d][%d] = %s\n", nlines, k, q);
            prevj = j;
            ++k;
          }
      }
      if (k > g_maxrf)
        g_maxrf = k;
      ++nlines;
    }
  }
  return nlines;
}

void writefields(int nlines)
{
  for (int i = 0; i != nlines; ++i) {
    for (int j = 0; j != MAXFIELDS; ++j) {
      if (mat[i][j])
        printf("%s\t", mat[i][j]);
      else
        break;
    }
    putchar('\n');
  }
}

void init()
{
  g_opt[0] = 0;
  g_col[0] = 1;
}

void parse(int argc, char *argv[])
{
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] != '-')
      continue;
    int j = 0;
    int opt = 0;
    int prev_k = g_k;
    do {
      ++j;
      int col = 0;
      while (isdigit(argv[i][j])) {
        col = col * 10 + (argv[i][j] - '0');
        ++j;
      }
      g_col[g_k] = col;
      ++g_k;
    }
    while (argv[i][j] == ',');
    if (argv[i][j] == ':') {
      ++j;
      for (/* empty */; argv[i][j]; ++j) {
        switch (argv[i][j]) {
          case 'n':
            opt |= NUMERIC;
            break;
          case 'r':
            opt |= REVERSE;
            break;
          case 'f':
            opt |= FOLD;
            break;
          case 'd':
            opt |= DIR;
            break;
          default:
            fprintf(stderr, "unknown option %c\n", argv[i][j]);
            break;
        }
      }
      for (int i = prev_k; i != g_k; ++i) {
        g_opt[i] = opt;
      }
    }
    g_opt[g_k] = opt;
  }
  for (int i = 0; i != g_k; ++i) {
    fprintf(stderr, "opt[%d] = %d, col[%d] = %d\n", i, g_opt[i], i, g_col[i]);
  }
  fprintf(stderr, "g_k = %d\n", g_k);
}

int main(int argc, char *argv[])
{
  init();
  parse(argc, argv);
  int nlines;
  if ((nlines = readlines()) >= 0) {
    printf("nlines = %d\n", nlines);
    printf("g_maxrf = %d\n", g_maxrf);
    printf("----------------------------------------------------------------------------------\n");
    writefields(nlines);
    printf("----------------------------------------------------------------------------------\n");
    qsort_((void **)mat, 0, nlines - 1, (int (*)(void *, void *))gencmp);
    writefields(nlines);
    printf("----------------------------------------------------------------------------------\n");
    return 0;
  }
  else {
    printf("intput too big to sort\n");
    return 1;
  }
}
