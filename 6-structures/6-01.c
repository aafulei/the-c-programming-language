/* Exercise 6-1. Our version of getword does not properly handle underscores,
 * string constants, comments or preprocessor control lines. Writer a better
 * version. */

#include <ctype.h>
#include <stdio.h>

#define MAXWORD 100
#define BUFSIZE 100

static int g_buffer[BUFSIZE];
static int *g_next = g_buffer;
static int g_in_single_quotes = 0;
static int g_in_double_quotes = 0;
static int g_prev_word_type = 0;
static int g_escaping = 0;

enum word_type {
  SIG_QUOTE_OUT = 1,
  SIG_QUOTE_IN = 2,
  DBL_QUOTE_OUT = 3,
  DBL_QUOTE_IN = 4,
  BLOCK_COMMENT = 5,
  LINE_COMMENT = 6,
  PREPROCESSOR_CTRL_LINE = 7,
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
  if (c == '#' && !g_in_single_quotes && !g_in_double_quotes) {
    while ((c = getch()) != '\n')
      ;
    return PREPROCESSOR_CTRL_LINE;
  }
  if (g_in_single_quotes || g_in_double_quotes) {
    if (g_prev_word_type == '\\') {
      g_escaping = !g_escaping;
    }
    else {
      g_escaping = 0;
    }
  }
  if (!isalpha_(*w++ = c)) {
    *w = '\0';
    if (c == '\'' && !g_in_double_quotes && !g_escaping) {
      g_in_single_quotes = !g_in_single_quotes;
      g_prev_word_type = SIG_QUOTE_OUT + g_in_single_quotes;
      return g_prev_word_type;
    }
    if (c == '"' && !g_in_single_quotes && !g_escaping) {
      g_in_double_quotes = !g_in_double_quotes;
      g_prev_word_type = DBL_QUOTE_OUT + g_in_double_quotes;
      return g_prev_word_type;
    }
    g_prev_word_type = c;
    return g_prev_word_type;
  }
  for (; --lim > 0; w++) {
    if (!isalnum_(*w = getch())) {
      ungetch(*w);
      break;
    }
  }
  *w = '\0';
  g_prev_word_type = word[0];
  return g_prev_word_type;
}

int main()
{
  char word[MAXWORD + 1];
  int wt;
  while ((wt = getword(word, MAXWORD)) != EOF) {
    if (wt == BLOCK_COMMENT) {
      printf("------ block comment ----------------\n");  /* block comment */
    }
    else if (wt == LINE_COMMENT) {
      printf("------ line comment -----------------\n");   // line comment
    }
    else if (wt == PREPROCESSOR_CTRL_LINE) {
      printf("------ preprocessor control line ----\n");
    }
    else {
      if (wt == SIG_QUOTE_IN) {
        printf("------ inside single quotes -------\n");
      }
      else if (wt == DBL_QUOTE_IN) {
        printf("------ inside double quotes -------\n");
      }
      printf("%s\n", word);
      if (wt == SIG_QUOTE_OUT) {
        printf("------ out of single quotes -------\n");
      }
      else if (wt == DBL_QUOTE_OUT) {
        printf("------ out of double quotes -------\n");
      }
    }
  }
  return 0;
}
