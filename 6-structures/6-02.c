/* Exercise 6-2. Write a program that reads a C program and prints in
 * alphabetical order each group of variable names that are identical in the
 * first 6 characters, but different somewhere thereafter. Don't count words
 * within strings and comments. Make 6 a parameter that can be set from the
 * command line. */

/*
  gcc 6-02.c && ./a.out 3 < 6-02.c

  ...
  str (9)
      strcmp (3)
      strcpy (1)
      strlen (2)
      strncmp (2)
      strncpy (1)
  ...
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXWORD 100
#define BUFSIZE 100

static int g_buffer[BUFSIZE];
static int *g_next = g_buffer;

enum word_type {
  ERROR = 0,
  SINGLE_QUOTES = 1,
  DOUBLE_QUOTES = 2,
  BLOCK_COMMENT = 3,
  LINE_COMMENT = 4,
  PREPROCESSOR_CTRL_LINE = 5,
};

int getch(void)
{
  return g_next == g_buffer ? getchar() : *--g_next;
}

int ungetch(int c)
{
  return g_next == g_buffer + BUFSIZE ? EOF : (*g_next++ = c);
}

int peekch()
{
  int c = getch();
  ungetch(c);
  return c;
}

int isalpha_(int c)
{
  return isalpha(c) || c == '_';
}

int isalnum_(int c)
{
  return isalnum(c) || c == '_';
}

int getword(char *word, int lim)
{
  int c, getch(void);
  int ungetch(int);
  char *w = word;

  while (isspace(c = getch()))
    ;
  if (c == EOF) {
    return EOF;
  }
  if (c == '/' && peekch() == '*') {
    getch();
    while (1) {
      c = getch();
      if (c == EOF)
        return c;
      if (c == '*' && peekch() == '/') {
        getch();
        return BLOCK_COMMENT;
      }
    }
  }
  if (c == '/' && peekch() == '/') {
    getch();
    while (1) {
      c = getch();
      if (c == EOF)
        return c;
      if (c == '\n') {
        return LINE_COMMENT;
      }
    }
  }
  if (c == '#') {
    while ((c = getch()) != '\n')
      ;
    return PREPROCESSOR_CTRL_LINE;
  }
  if (c == '\'') {
    c = getch();
    if (c == '\\')
      getch();
    if ((c = getch()) != '\'')
      return ERROR;
    else
      return SINGLE_QUOTES;
  }
  if (c == '\"') {
    // we consider only one-character escape sequence here
    int escaping = 0;
    while (1) {
      c = getch();
      if (!escaping && c == '\\')
        escaping = 1;
      else
        escaping = 0;
      if (c == '\"' && !escaping)
        break;
    }
    return DOUBLE_QUOTES;
  }
  if (!isalpha_(*w++ = c)) {
    *w = '\0';
    return c;
  }
  for (; --lim > 0; w++) {
    if (!isalnum_(*w = getch())) {
      ungetch(*w);
      break;
    }
  }
  *w = '\0';
  return word[0];
}

struct node
{
  struct node *left;
  struct node *right;
  char *value;
  int count;
  struct node *list;
};

struct node *g_root = NULL;

#define MIN(x, y) ((x) < (y) ? (x) : (y))

const char *g_keywords[]
    = { "break",   "case",   "char",     "const",  "continue", "do",
        "double",  "else",   "enum",     "extern", "float",    "for",
        "goto",    "if",     "int",      "long",   "register", "return",
        "short",   "signed", "sizeof",   "static", "struct",   "switch",
        "typedef", "union",  "unsigned", "void",   "volatile", "while" };

const char **binary_search(const char *w)
{
  const char **b = g_keywords,
             **e = g_keywords + (sizeof(g_keywords) / sizeof(g_keywords[0])),
             **m;
  while (b != e) {
    m = b + (e - b) / 2;
    int x = strcmp(w, *m);
    if (x == 0)
      return m;
    if (x < 0)
      e = m;
    else
      b = m + 1;
  }
  return NULL;
}

int is_keyword(const char *w)
{
  return binary_search(w) != NULL;
}

struct node *addnode(char *w, struct node *d, int n)
{
  if (!d) {
    d = malloc(sizeof(struct node));
    d->left = d->right = d->list = NULL;
    d->count = 1;
    // categorized
    if (n) {
      d->value = malloc(MIN(n, strlen(w)) + 1);
      strncpy(d->value, w, n);
      d->list = addnode(w, d->list, 0);
    }
    // end point
    else {
      d->value = malloc(strlen(w) + 1);
      strcpy(d->value, w);
    }
    return d;
  }
  if (n) {
    if (strncmp(w, d->value, n) == 0) {
      ++d->count;
      d->list = addnode(w, d->list, 0);
    }
    else if (strncmp(w, d->value, n) < 0) {
      d->left = addnode(w, d->left, n);
    }
    else {
      d->right = addnode(w, d->right, n);
    }
    return d;
  }
  else {
    if (strcmp(w, d->value) == 0) {
      ++d->count;
    }
    else if (strcmp(w, d->value) < 0) {
      d->left = addnode(w, d->left, 0);
    }
    else {
      d->right = addnode(w, d->right, 0);
    }
    return d;
  }
}

void printnode(struct node *d)
{
  if (!d)
    return;
  printnode(d->left);
  printf("%s%s (%d)\n", (d->list ? "" : "\t\t"), d->value, d->count);
  if (d->list) {
    printnode(d->list);
  }
  printnode(d->right);
}

int main(int argc, char *argv[])
{
  int n;
  if (argc == 1 || (n = atoi(argv[1])) == 0 || n <= 0) {
    fprintf(stderr, "warning: usage is \"command [n=6], where n > 0\"\n");
    n = 6;
  }
  fprintf(stderr, "get n = %d\n", n);

  char word[MAXWORD + 1];
  int wt;
  while ((wt = getword(word, MAXWORD)) != EOF) {
    if (wt == BLOCK_COMMENT) {
      printf("------ block comment ----------------\n"); /* block comment */
    }
    else if (wt == LINE_COMMENT) {
      printf("------ line comment -----------------\n"); // line comment
    }
    else if (wt == PREPROCESSOR_CTRL_LINE) {
      printf("------ preprocessor control line ----\n");
    }
    else if (wt == SINGLE_QUOTES) {
      printf("------ single quotes ----------------\n");
    }
    else if (wt == DOUBLE_QUOTES) {
      printf("------ double quotes ----------------\n");
    }
    else if (!isalpha_(wt)) {
      printf("\033[2m%s\033[0m\n", word);
    }
    else {
      if (is_keyword(word))
        printf("\033[32m%s\033[0m\n", word);
      else {
        printf("%s\n", word);
        g_root = addnode(word, g_root, n);
      }
    }
  }
  printf("================================================================\n");
  printnode(g_root);
  return 0;
}
