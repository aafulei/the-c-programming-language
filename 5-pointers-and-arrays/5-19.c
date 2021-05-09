// NOT FINISHED. JUST RUNABLE.

/* Exercise 5-19. Modify undcl so that it does not add redundant parentheses to
 * declarations. */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXSIZE 1000
#define BUFSIZE 100

static char g_buffer[BUFSIZE];
static char *g_next = g_buffer;

int getch()
{
  if (g_next == g_buffer)
    return getchar();
  else
    return *--g_next;
}

int ungetch(int c)
{
  if (g_next < g_buffer + BUFSIZE)
    return (*g_next++ = c);
  else
    return EOF;
}

#define MAXTOKEN 1000

enum { NAME, PARENTHESES, BRACKETS };


int g_token_type;
char g_token[100];
char g_dcl[1000];

int get_token_type()
{
  char *p = g_token;
  int c = getch();
  while (c == ' ' || c == '\t')
    c = getch();
  if (c == '(') {
    if ((c = getch()) == ')') {
      *p++ = '(';
      *p++ = ')';
      g_token_type = PARENTHESES;
    }
    else {
      ungetch(c);
      g_token_type = '(';
    }
  }
  else if (c == '[') {
    for (*p++ = c; (*p++ = getch()) != ']'; /* empty */)
      ;
    g_token_type = BRACKETS;
  }
  else if (isalpha(c)) {
    for (*p++ = c; isalnum(c = getch()) || c == '_'; *p++ = c)
      ;
    ungetch(c);
    g_token_type = NAME;
  }
  else {
    g_token_type = c;
  }
  *p = '\0';
  return g_token_type;
}

int main()
{
  int type;
  char temp[MAXTOKEN];
  while (get_token_type() != EOF) {
    strcpy(g_dcl, g_token);
    while ((type = get_token_type()) != '\n') {
      if (type == PARENTHESES || type == BRACKETS)
        strcat(g_dcl, g_token);
      else if (type == '*') {
        sprintf(temp, "(*%s)", g_dcl);
        strcpy(g_dcl, temp);
      }
      else if (type == NAME) {
        sprintf(temp, "%s %s", g_token, g_dcl);
        strcpy(g_dcl, temp);
      }
      else {
        printf("invalid input at %s\n", g_token);
      }
    }
    printf("%s\n", g_dcl);
  }
  return 0;
}
