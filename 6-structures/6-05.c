// not finished

/* Exercise 6-5. Write a function undef that will remove a name and definition
 * from the table maintained by lookup and install. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASHSIZE 101
#define MAXWORD  50
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
    t[i] = *t++;
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
    struct nlist *p = hashtab[hash(name)];
    if (!p) {
      if ((p = malloc(sizeof(struct nlist))) == NULL)
        return NULL;
    }
    else {
      while (p->next)
        p = p->next;
      if ((p->next = malloc(sizeof(struct nlist))) == NULL)
        return NULL;
      p = p->next;
    }
    printf("here %d\n", p!=NULL);
    p->name = strdup_(name);
    p->defn = strdup_(defn);
    printf("here p->name = %s\n", p->name);
    printf("here p->defn = %s\n", p->defn);
  }
  return p;
}

struct nlist *remove_(char *name)
{

}

int isalpha_(char c)
{
  return isalpha(c) || c == '_';
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

int getword(char *word, int lim)
{
  int c;
  while (isspace(c = getch()))
    ;
  ungetch(c);
  char *p = word;
  for (int n = 0; (c = getch()) != EOF && !isspace(c); ++n) {
    if (n < lim)
      *p++ = c;
  }
  *p = '\0';
  ungetch(c);
  return *word ? word[0] : EOF;
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

int main()
{
  int wt;
  char word[MAXWORD + 1];

  struct nlist *p = install("GOOD", "really bad");
  if (!p) {
    printf("bad alloc\n");
    return 1;
  }
  printf("check: p->name = %s, p->defn = %s\n", p->name, p->defn);
  debug();
  // while ((wt = getword(word, MAXWORD)) != EOF) {
  //   if (isalpha_(word[0])) {
  //     struct nlist *p;
  //     if (p = lookup(word))
  //       printf("get %s ==> %s\n", p->name, p->defn);
  //     else
  //       printf("get %s\n", word);
  //   }
  // }
  return 0;
}
