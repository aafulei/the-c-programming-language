/* Exercise 6-4. Write a program that prints the distinct words in its input
 * sorted into decreasing order of frequency of occurrence. Precede each word
 * by its count. */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXWORD 100
#define BUFSIZE 100

int g_buffer[BUFSIZE];
int *g_next = g_buffer;

int getch()
{
  return g_next == g_buffer ? getchar() : *--g_next;
}

int ungetch(int c)
{
  return g_next == g_buffer + BUFSIZE ? EOF : (*g_next++ = c);
}

int getword(char *w, int lim)
{
  static int line = 1;
  int c;
  while (isspace(c = getch()) && c != '\n')
    ;
  ungetch(c);
  int n = 0;
  while ((c = getch()) != EOF && (isalnum(c) || c == '_')) {
    if (n++ < lim)
      *w++ = c;
  }
  *w = '\0';
  if (c == EOF)
    return EOF;
  else if (c == '\n')
    return line++;
  else
    return line;
}

struct node
{
  struct node *left;
  struct node *right;
  char *value;
  int count;
};

int g_num_nodes = 0;

struct node *addnode(struct node *d, const char *w)
{
  if (!d) {
    d = malloc(sizeof(struct node));
    ++g_num_nodes;
    d->left = d->right = NULL;
    d->value = malloc(strlen(w) + 1);
    strcpy(d->value, w);
    d->count = 1;
  }
  else {
    int res = strcmp(w, d->value);
    if (res == 0) {
      ++d->count;
    }
    else if (res < 0) {
      d->left = addnode(d->left, w);
    }
    else {
      d->right = addnode(d->right, w);
    }
  }
  return d;
}

struct node *flatten(struct node *d, struct node *flat)
{
  if (!d)
    return flat;
  struct node *next;
  next = flatten(d->left, flat);
  next->value = d->value;
  next->count = d->count;
  ++next;
  next = flatten(d->right, next);
  return next;
}

void swap(struct node *a, struct node *b)
{
  if (a == b)
    return;
  struct node d = *a;
  *a = *b;
  *b = d;
}

void quicksort(struct node *b, struct node *e)
{
  if (e - b <= 1)
    return;
  struct node *p = b + (e - b) / 2;
  swap(b, p);
  p = b;
  for (struct node *q = p + 1; q != e; ++q) {
    if (q->count > b->count) {
      swap(q, p + 1);
      ++p;
    }
  }
  swap(b, p);
  quicksort(b, p);
  quicksort(p + 1, e);
}

int main()
{
  char word[MAXWORD + 1];
  int line;
  struct node *root = NULL;
  while ((line = getword(word, MAXWORD)) != EOF) {
    if (word[0]) {
      printf("get %s at line %d\n", word, line);
      root = addnode(root, word);
    }
  }
  printf("================================================================\n");
  printf("There are %d distinct words\n", g_num_nodes);

  struct node *flat = malloc(g_num_nodes * sizeof(struct node));
  flatten(root, flat);

  for (int i = 0; i != g_num_nodes; ++i) {
    printf("%d: %s\n", flat[i].count, flat[i].value);
  }
  quicksort(flat, flat + g_num_nodes);
  printf("================================================================\n");
  for (int i = 0; i != g_num_nodes; ++i) {
    printf("%d: %s\n", flat[i].count, flat[i].value);
  }
  return 0;
}
