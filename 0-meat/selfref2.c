// 21/04/30 = Fri

// gcc selfref2.c -pedantic -Wall -Wextra

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXWORD 100

struct node
{
  char *word;
  int count;
  struct node *left;
  struct node *right;
};

struct node *addtree(struct node *, const char *);
struct node *talloc();
size_t strlen(const char *);
int strcmp(const char *, const char *);
char *strdup(const char *);
int getword(char *, int);
void printtree(struct node *);

struct node *addtree(struct node *root, const char *word)
{
  if (root) {
    int res = strcmp(word, root->word);
    if (res == 0)
      ++root->count;
    else if (res < 0)
      root->left = addtree(root->left, word);
    else
      root->right = addtree(root->right, word);
  }
  else {
    root = talloc();
    root->word = strdup(word);
    root->count = 1;
    root->left = root->right = NULL;
  }
  return root;
}

struct node *talloc()
{
  return malloc(sizeof(struct node));
}

size_t strlen(const char *s)
{
  size_t len = 0;
  while (*s++ != '\0')
    ++len;
  return len;
}

int strcmp(const char *s, const char *t)
{
  for (int d; /* empty */; ++s, ++t) {
    d = *(unsigned char *)s - *(unsigned char *)t;
    if (d || *s == '\0')
      return d;
  }
}

char *strdup(const char *s)
{
  char *dup = malloc(strlen(s) + 1);
  for (char *p = dup; (*p++ = *s++) != '\0'; /* empty */)
    /* empty */;
  return dup;
}

int getword(char *s, int lim)
{
  int has_next = 1;
  for (int i = 0, c; /* empty */; ++i) {
    c = getchar();
    if (c == EOF) {
      has_next = 0;
      break;
    }
    if (!isalpha(c)) {
      break;
    }
    if (i < lim)
      *s++ = c;
  }
  *s = '\0';
  return has_next;
}

void printtree(struct node *root)
{
  if (!root)
    return;
  printtree(root->left);
  printf("%s: %d\n", root->word, root->count);
  printtree(root->right);
}

int main()
{
  struct node *root = NULL;
  char word[MAXWORD + 1];
  while (getword(word, MAXWORD)) {
    if (word[0] != '\0')
      root = addtree(root, word);
  }
  printtree(root);
  return 0;
}
