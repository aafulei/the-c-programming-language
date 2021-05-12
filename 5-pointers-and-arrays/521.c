// void (*foo(int, double (*)(long)))(float)
// foo:  function of int and pointer to function of long returning double returning pointer to function of float returning void

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 1000
#define BUFSIZE 100

#define OK                                        0
#define ERR_MISSING_CLOSING_PARENTHESIS           1
#define ERR_EXPECT_NAME_OR_DCL_IN_PARENTHESES     2
#define ERR_SYNTAX_ERROR                          3
#define ERR_PAR_MISSING_CLOSING_PARENTHESIS       4
#define ERR_PAR_EXPECT_NAME_OR_DCL_IN_PARENTHESES 5
#define ERR_PAR_SYNTAX_ERROR                      6
#define ERR_NOT_IMPLEMENTED                       7

static char g_buffer[BUFSIZE];
static char *g_next = g_buffer;

char g_token[100];
enum { NAME, PARENTHESES, BRACKETS };
int g_token_type;
char g_name[100];
char g_dcl[1000];
char g_type[100];
char g_par_name[100];


int parse();
int dcl();
int ddcl();
void clean();


int getch()
{
  int res;
  if (g_next == g_buffer)
    res = getchar();
  else
    res = *--g_next;
#ifdef DEBUG
  fprintf(stderr, "getch \033[32m%c\033[0m: g_token = %s\n", res, g_token);
#endif
  return res;
}

int ungetch(int c)
{
#ifdef DEBUG
  fprintf(stderr, "unget \033[33m%c\033[0m: g_token = %s\n", c, g_token);
#endif
  if (g_next < g_buffer + BUFSIZE)
    return (*g_next++ = c);
  else
    return EOF;
}

int peek_token_type()
{
  char peeked[100];
  peeked[0] = '\0';
  char *p = peeked;
  int c = getch();
  int n = 1;
  while (c == ' ' || c == '\t') {
    c = getch();
  }
  if (isalpha(c)) {
    for (*p++ = c; isalnum(c = getch()) || c == '_'; *p++ = c, ++n)
      ;
    *p = '\0';
    ungetch(c);
    char *q = peeked;
    while (*q)
      ++q;
    for (--q; q >= peeked; --q)
      ungetch(*q);
    return 1;
  }
  else if (c == ')') {
    ungetch(c);
    return 1;
  }
  else {
    ungetch(c);
    return 0;
  }
}

int get_token_type()
{
  char *p = g_token;
  int c = getch();
  while (c == ' ' || c == '\t')
    c = getch();
  if (c == '(') {
    *p++ = c;
    if (peek_token_type()) {
      g_token_type = PARENTHESES;
    }
    else {
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
    *p++ = c;
    g_token_type = c;
  }
  *p = '\0';
  return g_token_type;
}

// when entering and returning from ddcl(), it has got one more token that does
// not belong to itself (ddcl part)

int dcl()
{
  int res;
  int ns = 0;
  while (get_token_type() == '*') {
    ++ns;
  }
  if ((res = ddcl()) != OK)
    return res;
  while (ns-- > 0)
    strcat(g_dcl, "pointer to ");
  return res;
}

int ddcl()
{
  int res;
  if (g_token_type == NAME) {
    if (strlen(g_name) == 0)
      strcpy(g_name, g_token);
  }
  else if (g_token_type == '(') {
    if ((res = dcl()) != OK)
      return res;
    if (g_token_type != ')')
      return ERR_MISSING_CLOSING_PARENTHESIS;
  }
  else if (g_token_type == ')' || g_token_type == ',') {
    return OK;
  }
  else {
    return ERR_EXPECT_NAME_OR_DCL_IN_PARENTHESES;
  }

  for (int tt = get_token_type(); tt == PARENTHESES || tt == BRACKETS;
       tt = get_token_type()) {
    if (tt == PARENTHESES) {
      strcat(g_dcl, "function of ");
      while (g_token_type != ')') {
        if ((res = parse()) != OK)
          return res;
        if (g_token_type == ',')
          strcat(g_dcl, " and ");
      }
      strcat(g_dcl, " returning ");
    }
    else {
      strcat(g_dcl, "array");
      strcat(g_dcl, g_token);
      strcat(g_dcl, " of ");
    }
  }
  return OK;
}

void error_handle(int res)
{
  switch (res) {
    case ERR_MISSING_CLOSING_PARENTHESIS:
      printf("[error] missing closing parenthesis ')'\n");
      break;
    case ERR_EXPECT_NAME_OR_DCL_IN_PARENTHESES:
      printf("[error] expect name or (dcl)\n");
      break;
    case ERR_SYNTAX_ERROR:
      printf("[error] syntax error, expect ending with ; or newline\n");
      break;
    case ERR_PAR_MISSING_CLOSING_PARENTHESIS:
      printf("[error] para missing closing parenthesis ')'\n");
      break;
    case ERR_PAR_EXPECT_NAME_OR_DCL_IN_PARENTHESES:
      printf("[error] para expect name or (dcl)\n");
      break;
    case ERR_PAR_SYNTAX_ERROR:
      printf("[error] para syntax error, expect ending with , or )\n");
      break;
    case ERR_NOT_IMPLEMENTED:
      printf("[error] not implemented\n");
      break;
    default:
      printf("[error] unknown error code %d\n", res);
      break;
  }
  fprintf(
      stderr,
      "[error] g_name = %s, g_dcl = %s, g_type = %s, g_token = %s, "
      "g_token_type = %d, g_par_name = %s\n",
      g_name, g_dcl, g_type, g_token, g_token_type, g_par_name);
  // ignore all trailing characters in the line
  if (g_token_type != '\n') {
    while (getch() != '\n')
      ;
  }
}

int parse()
{
  get_token_type();
  if (g_token_type == ')') {
    strcat(g_dcl, "void");
    return OK;
  }

  char a_type[100];
  strcpy(a_type, g_token);

  int res = dcl();
  // if (res == OK && (g_token_type != ';' && g_token_type != '\n'))
  //   res = ERR_SYNTAX_ERROR;
  if (res == OK)
    strcat(g_dcl, a_type);
  return res;
}

void clean()
{
  g_dcl[0] = '\0';
  g_token[0] = '\0';
  g_name[0] = '\0';
}

int main()
{
  int res;
  while (1) {
    if ((res = parse()) != OK)
      error_handle(res);
    printf("%s: ", g_name);
    printf("%s\n", g_dcl);
    clean();
  }
  return 0;
}
