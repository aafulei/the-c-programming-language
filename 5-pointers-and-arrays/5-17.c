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

// ./a.out -:5
// ./a.out -n:5
// ./a.out -:2,1 -nr:4

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

static char *matptr[MAXLINES][MAXFIELDS];

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

int readlines(char *lineptr[], int nlines, int *nmaxfields);
void writelines(char *lineptr[], int nlines);

void qsort_(void *lineptr[], int left, int right, int (*comp)(void *, void *));

int gencmp(const char **, const char **);

int numcmp(const char *, const char *);
int rstrcmp(const char *, const char *);
int rchacmp(const char *, const char *);

// int (*rev(int (*f)(void *, void *), int r))(void *, void *) {}

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

// void qqsort_(void ***v, int left, int right, int (*comp)(void **, void **))
// {
//   int i, last;
//   void swap2(void ***, int, int);
//   if (left >= right)
//     return;
//   swap2(v, left, (left + right) / 2);
//   last = left;
//   for (i = left + 1; i <= right; ++i)
//     if ((*comp)(v[i], v[left]) < 0)
//       swap2(v, ++last, i);
//   swap2(v, left, last);
//   qqsort_(v, left, last - 1, comp);
//   qqsort_(v, last + 1, right, comp);
// }

void swap(void *v[], int i, int j)
{
  int *t;
  t = v[i];
  v[i] = v[j];
  v[j] = t;
}

void swap2(void **v[], int i, int j)
{
  printf("swap2ing %d with %d\n", i, j);
  void **t;
  t = v[i];
  v[i] = v[j];
  v[j] = t;
  printf("return swap2ing %d with %d\n", i, j);
  // writefields(7, 4);
}


int field_cmp(const char *s, const char *t, int opt, int col)
{
  printf("compare %s vs %s\n", s, t);
  int (*fp)(const char *, const char *);
  // set global options
  g_options = opt;
  if (opt & NUMERIC)
    fp = numcmp;
  else if (opt & FOLD)
    fp = rchacmp;
  else
    fp = rstrcmp;
  return fp(s, t);
}

int gencmp(const char **s, const char **t)
{
  for (int i = 0, res; i != g_nf; ++i) {
    if ((res = field_cmp(s[i], t[i], g_opt[i], g_col[i])) != 0)
      return res;
  }
  return 0;
}

int field_cmp2(const char **s, const char **t, int i)
{
  // printf("compare %s vs %s\n", s[g_col[i]-1], t[g_col[i]-1]);
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


int gencmp2(const char **s, const char **t)
{
  for (int i = 0, res; i != g_k; ++i) {
    if ((res = field_cmp2(s, t, i)) != 0)
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

int readlines(char *lineptr[], int maxlines, int *nmaxfields)
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
      lineptr[nlines] = p;
      for (int j = 0, prevj = 0, k = 0, ready = 0; j != len; ++j) {
          if (line[j] == '\t')
            ready = 1;
          else if (ready) {
            ready = 0;
            char *q = alloc(j - prevj + 1);
            strncpy(q, line + prevj, j - prevj);
            // printf("nlines = %d, k = %d, %s\n", nlines, k, q);
            matptr[nlines][k] = q;
            prevj = j;
            ++k;
          }
          if (k-1 > *nmaxfields)
            *nmaxfields = k;
      }

      ++nlines;
    }
  return nlines;
}

int readlines2(void)
{
  int len, nlines;
  char *p, line[MAXLEN];
  nlines = 0;
  while ((len = getline_(line, MAXLEN)) > 0) {
    if (nlines >= MAXLINES || (p = alloc(len)) == NULL)
      return -1;
    else {
      line[len - 1] = '\0';
      strcpy(p, line);
      mat[nlines] = calloc(g_nf, sizeof(char *));

      // printf("deal with %s\n", line);
      for (int j = 0, prevj = 0, k = 0, ready = 0; j != len; ++j) {
          if (line[j] == '\t')
            ready = 1;
          else if (ready) {
            ready = 0;
            char *q = alloc(j - prevj + 1);
            strncpy(q, line + prevj, j - prevj);
            mat[nlines][k] = q;
            // printf("mat got %s\n", mat[nlines][k]);
            prevj = j;
            ++k;
            if (k == g_nf) {
              break;
            }
          }
          else if (line[j] == '\0') {
            char *q = alloc(j - prevj + 1);
            strncpy(q, line + prevj, j - prevj);
            mat[nlines][k] = q;
            // printf("mat got %s\n", mat[nlines][k]);
            prevj = j;
            ++k;
            if (k == g_nf) {
              break;
            }
          }
      }
      ++nlines;
    }
  }
  return nlines;
}

int readlines3(void)
{
  int len, nlines;
  char *p, line[MAXLEN];
  nlines = 0;
  while ((len = getline_(line, MAXLEN)) > 0) {
    if (nlines >= MAXLINES || (p = alloc(len)) == NULL)
      return -1;
    else {
      line[len - 1] = '\0';
      strcpy(p, line);
      mat[nlines] = calloc(MAXFIELDS, sizeof(char *));
      for (int j = 0, prevj = 0, k = 0, ready = 0; j != len; ++j) {
          if (line[j] == '\t')
            ready = 1;
          else if (ready) {
            ready = 0;
            char *q = alloc(j - prevj + 1);
            strncpy(q, line + prevj, j - prevj);
            mat[nlines][k] = q;
            prevj = j;
            ++k;
            // if (k == g_nf) {
            //   break;
            // }
          }
          else if (line[j] == '\0') {
            char *q = alloc(j - prevj + 1);
            strncpy(q, line + prevj, j - prevj);
            mat[nlines][k] = q;
            // printf("mat got %s\n", mat[nlines][k]);
            prevj = j;
            ++k;
            // if (k == g_nf) {
            //   break;
            // }
          }
      }
      ++nlines;
    }
  }
  return nlines;
}



void writelines(char *lineptr[], int nlines)
{
  int i;
  for (i = 0; i < nlines; ++i)
    printf("%s\n", lineptr[i]);
}

void writefields(int nlines, int nfields)
{
  for (int i = 0; i != nlines; ++i) {
    for (int j = 0; j != nfields; ++j) {
      printf("%s\t", matptr[i][j]);
    }
    putchar('\n');
  }
}

void writefields2(int nlines, int nfields)
{
  for (int i = 0; i != nlines; ++i) {
    for (int j = 0; j != nfields; ++j) {
      printf("%s\t", mat[i][j]);
    }
    putchar('\n');
  }
}

void writefields3(int nlines)
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
      // g_opt[g_k] = opt;
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
  if ((nlines = readlines3()) >= 0) {
    printf("nlines = %d\n", nlines);
    printf("--------------------------------------------------------------\n");
    writefields3(nlines);
    printf("--------------------------------------------------------------\n");
    qsort_((void **)mat, 0, nlines - 1, (int (*)(void *, void *))gencmp2);
    writefields3(nlines);
    return 0;
  }
  else {
    printf("intput too big to sort\n");
    return 1;
  }
}
