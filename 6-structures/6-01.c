/* Exercise 6-1. Our version of getword does not properly handle underscores,
 * string constants, comments or preprocessor control lines. Writer a better
 * version. */

#include <ctype.h>
#include <stdio.h>

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
    int lastc = c;
    int escaping = 0;
    while (1) {
      c = getch();
      if (c == '\\')
        escaping = !escaping;
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

int main()
{
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
    else {
      printf("%s\n", word);
    }
  }
  return 0;
}
