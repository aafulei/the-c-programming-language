#include <ctype.h>
#include <stddef.h>
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

struct node *talloc();
void treeprint(struct node *);
int getword(char *, int);
size_t strlen(char *);
int strcmp(char *, char *);
char *strdup(char *);

struct node *addtree(struct node *p, char *w)
{
  if (p) {
    int cmp = strcmp(w, p->word);
    if (cmp == 0)
      ++p->count;
    else if (cmp < 0)
      p->left = addtree(p->left, w);
    else
      p->right = addtree(p->right, w);
  }
  else {
    p = talloc();
    p->word = strdup(w);
    p->count = 1;
    p->left = p->right = NULL;
  }
  return p;
}

struct node *talloc()
{
  return (struct node *)malloc(sizeof(struct node));
}

size_t strlen(char *s)
{
  size_t len = 0;
  while ((*s++) != '\0')
    ++len;
  return len;
}

int strcmp(char *a, char *b)
{
  for (int cmp; /* empty */; ++a, ++b) {
    switch ((*a != '\0') + (*b != '\0') * 2) {
      case 0:
        return 0;
      case 1:
        return 1;
      case 2:
        return -1;
      case 3: {
        if ((cmp = *a - *b) != 0)
          return cmp;
        else
          break;
      }
    }
  }
}

char *strdup(char *orig)
{
  char *dup = malloc(strlen(orig) + 1);
  char *p = dup;
  while ((*p++ = *orig++) != '\0')
    ;
  return dup;
}

// read a word, write at most lim characters to where p points
// return if EOF has been reached
// note: by "word" we consider the simplest case: a word is a sequence of
// consecutive english characters, and a word can be an empty one if the next
// character is not an english character
int getword(char *p, int lim)
{
  int end = 0;
  for (int c, n = 0;; ++n) {
    c = getchar();
    if (c == EOF) {
      end = 1;
      break;
    }
    if (!isalpha(c))
      break;
    if (n < lim)
      *p++ = c;
  }
  *p = '\0';
  return end;
}

void treeprint(struct node *root)
{
  if (root == NULL)
    return;
  treeprint(root->left);
  printf("%s: %d\n", root->word, root->count);
  treeprint(root->right);
}

int main()
{
  struct node *root = NULL;
  char word[MAXWORD + 1];
  int end;
  do {
    end = getword(word, MAXWORD);
    if (word[0] != '\0')
      root = addtree(root, word);
  }
  while (!end);
  treeprint(root);
  return 0;
}
