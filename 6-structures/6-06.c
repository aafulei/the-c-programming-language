/* Exercise 6-6. Implement a simple version of the #define processor (i.e., no
 * arguments) suitable for use with C programs, based on the routines of this
 * section. You may also find getch and ungetch helpful.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASHSIZE 101
#define MAXWORD  50
#define MAXLINE  1000
#define BUFSIZE  100

struct nlist
{
  struct nlist *next;
  char *name;
  char *defn;
};

static struct nlist *hashtab[HASHSIZE];
unsigned hash(char *s)

{
  unsigned h = 0;
  while (*s) {
    h += h * 31 + *s++;
  }
  return h % HASHSIZE;
}

char *strdup_(char *s)
{
  int n = 0;
  for (char *p = s; *p; ++p)
    ++n;
  char *t = malloc(n + 1);
  if (!t)
    return NULL;
  for (int i = 0; i <= n; ++i)
    t[i] = *s++;
  return t;
}

struct nlist *lookup(char *name)
{
  struct nlist *p = hashtab[hash(name)];
  for (/* empty */; p; p = p->next) {
    if (strcmp(p->name, name) == 0)
      return p;
  }
  return NULL;
}

struct nlist *install(char *name, char *defn)
{
  struct nlist *p = lookup(name);
  if (p) {
    free(p->defn);
    p->defn = strdup_(defn);
  }
  else {
    p = hashtab[hash(name)];
    if (!p) {
      if ((hashtab[hash(name)] = malloc(sizeof(struct nlist))) == NULL)
        return NULL;
      p = hashtab[hash(name)];
    }
    else {
      while (p->next)
        p = p->next;
      if ((p->next = malloc(sizeof(struct nlist))) == NULL)
        return NULL;
      p = p->next;
    }
    p->name = strdup_(name);
    p->defn = strdup_(defn);
  }
  return p;
}

int undef(char *name)
{
  struct nlist *q = hashtab[hash(name)];
  if (!q)
    return 0;
  if (strcmp(q->name, name) == 0) {
    hashtab[hash(name)] = q->next;
  }
  else {
    for (struct nlist *p = q; q; p = q, q = q->next) {
      if (strcmp(q->name, name) == 0) {
        p->next = q->next;
        break;
      }
    }
  }
  free(q);
  return 1;
}

int isalpha_(char c)
{
  return isalpha(c) || c == '_';
}

int isalnum_(char c)
{
  return isalnum(c) || c == '_';
}


static int g_buffer[BUFSIZE];
static int *g_next = g_buffer;

int getch()
{
  return g_next == g_buffer ? getchar() : *--g_next;
}

int ungetch(int c)
{
  return g_next == g_buffer + BUFSIZE ? EOF : (*g_next++ = c);
}

void debug()
{
  for (int i = 0; i != HASHSIZE; ++i) {
    struct nlist *p = hashtab[i];
    while (p) {
      printf("hashtab[%d]: %s ==> %s\n", i, p->name, p->defn);
      p = p->next;
    }
  }
}

int getline_(char *line, int lim)
{
  int c;
  int n = 0;
  while ((c = getchar()) != EOF && c != '\n') {
    if (n++ < lim - 1)
      *line++ = c;
  }
  if (c == EOF)
    return 0;
  *line++ = '\n';
  *line = '\0';
  return 1;
}

int is_define(char *p, char *name, char *defn, int lim)
{
  while (isspace(*p))
    ++p;
  if (strncmp(p, "#define", 7) == 0) {
    p += 7;
    while (isspace(*p))
      ++p;
    int n = 0;
    while (isalnum_(*p)) {
      if (n++ < lim)
        *name++ = *p;
      ++p;
    }
    *name = '\0';
    while (isspace(*p))
      ++p;
    n = 0;
    while (isalnum_(*p)) {
      if (n++ < lim)
        *defn++ = *p;
      ++p;
    }
    *defn = '\0';
    return 1;
  }
  return 0;
}

enum { ALNUM, SYMBOL, WHITESPACE, NEWLINE };


int getstate(int c)
{
  if (c == '\n')
    return NEWLINE;
  if (isspace(c))
    return WHITESPACE;
  if (isalnum_(c))
    return ALNUM;
  return SYMBOL;
}

char *nextword(char *line, char *word, int lim)
{
  int state = getstate(*line);
  *word++ = *line;
  if (state == NEWLINE) {
    *word = '\0';
    return NULL;
  }
  int n = 0;
  while (1) {
    int c = *++line;
    if (getstate(c) != state) {
      break;
    }
    else {
      if (n++ < lim)
        *word++ = c;
    }
  }
  *word = '\0';
  return line;
}

int main()
{
  char word[MAXWORD + 1];
  char name[MAXWORD + 1];
  char defn[MAXWORD + 1];
  char line[MAXLINE + 1];

  int lineno = 0;
  while (getline_(line, MAXLINE)) {
    ++lineno;
    if (is_define(line, name, defn, MAXWORD)) {
      // printf("%s --> %s\n", name, defn);
      install(name, defn);
      printf("%s", line);
    }
    else {
      char *p = line;
      while (p = nextword(p, word, MAXWORD)) {
        // printf("[%d] %s\n", lineno, word);
        struct nlist *np;
        if (np = lookup(word)) {
          printf("%s", np->defn);
        }
        else {
          printf("%s", word);
        }
      }
      putchar('\n');
    }
  }
  return 0;
}
