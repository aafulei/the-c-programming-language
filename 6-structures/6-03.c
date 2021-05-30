/* Exercise 6-3. Write a cross-referencer that prints a list of all words in a
 * document, and for each word, a list of the line numbers on which it occurs.
 * Remove noise words like "the", "and", and so on. */

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
  int capacity;
  int *list;
};

struct node *addnode(struct node *d, const char *w, int n)
{
  if (!d) {
    d = malloc(sizeof(struct node));
    d->left = d->right = NULL;
    d->value = malloc(strlen(w) + 1);
    strcpy(d->value, w);
    d->count = 1;
    d->capacity = 8;
    d->list = malloc(d->capacity * sizeof(int));
    d->list[d->count - 1] = n;
  }
  else {
    int res = strcmp(w, d->value);
    if (res == 0) {
      ++d->count;
      if (d->count > d->capacity) {
        d->capacity *= 2;
        int *new_list = malloc(d->capacity * sizeof(int));
        for (int i = 0; i != d->capacity / 2; ++i) {
          new_list[i] = d->list[i];
        }
        int *old_list = d->list;
        d->list = new_list;
        free(old_list);
      }
      d->list[d->count - 1] = n;
    }
    else if (res < 0) {
      d->left = addnode(d->left, w, n);
    }
    else {
      d->right = addnode(d->right, w, n);
    }
  }
  return d;
}

void printnode(struct node *d)
{
  if (!d)
    return;
  printnode(d->left);
  printf("%-20s", d->value);
  for (int i = 0; i != d->count; ++i) {
    printf("%d%c", d->list[i], (i == d->count - 1 ? '\n' : ' '));
  }
  printnode(d->right);
}

#define numof(a) (sizeof(a) / sizeof((a)[0]))

const char *g_noise[] = {
  "a",         "about", "after",    "all",       "am",    "an",    "and",
  "any",       "are",   "as",       "at",        "away",  "be",    "been",
  "before",    "being", "but",      "by",        "can",   "could", "did",
  "do",        "don",   "done",     "down",      "for",   "from",  "had",
  "has",       "have",  "he",       "her",       "here",  "hers",  "herself",
  "herselves", "him",   "himself",  "himselves", "his",   "how",   "i",
  "if",        "in",    "into",     "is",        "it",    "its",   "itself",
  "itselve",   "may",   "me",       "might",     "mine",  "my",    "myself",
  "myselves",  "no",    "not",      "o",         "of",    "on",    "or",
  "our",       "ours",  "ourself",  "ourselves", "out",   "over",  "s",
  "shall",     "she",   "should",   "t",         "that",  "the",   "their",
  "theirs",    "them",  "then",     "there",     "they",  "this",  "through",
  "to",        "under", "up",       "upon",      "us",    "was",   "we",
  "were",      "what",  "when",     "where",     "which", "who",   "why",
  "will",      "with",  "within",   "without",   "would", "yes",   "you",
  "your",      "yours", "yourself", "yourselves"
};

const char **binary_search(const char *b[], const char *e[], const char *w)
{
  const char **m;
  while (b != e) {
    m = b + (e - b) / 2;
    int res = strcmp(w, *m);
    if (res == 0)
      return m;
    if (res < 0)
      e = m;
    else
      b = m + 1;
  }
  return NULL;
}

int is_noise(const char *word)
{
  char w[MAXWORD + 1];
  strncpy(w, word, MAXWORD);
  char *p = w;
  while (*p != '\0') {
    if ('A' <= *p && *p <= 'Z')
      *p = *p - 'A' + 'a';
    ++p;
  }
  return binary_search(g_noise, g_noise + numof(g_noise), w) != NULL;
}

int main()
{
  char word[MAXWORD + 1];
  int line;
  struct node *root = NULL;
  while ((line = getword(word, MAXWORD)) != EOF) {
    if (word[0] && !is_noise(word)) {
      // printf("get %s at line %d\n", word, line);
      root = addnode(root, word, line);
    }
  }
  printf("================================================================\n");
  printnode(root);
  return 0;
}
