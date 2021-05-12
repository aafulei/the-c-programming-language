/* Exercise 5-20. Expand dcl to handle declarations with function argument
 * types, qualifiers like const, and so on. */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

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

#define OK                                    0
#define ERR_MISSING_CLOSING_PARENTHESIS       1
#define ERR_EXPECT_NAME_OR_DCL_IN_PARENTHESES 2
#define ERR_SYNTAX_ERROR                      3

enum { NAME, PARENTHESES, BRACKETS };
int g_token_type;
char g_token[100];
char g_name[100];
char g_dcl[1000];
char g_type[100];
char g_par_type[100];
char g_par_dcl[1000];
char g_par_name[100];
int g_paren_type = 0;

int get_token_type();
int dcl();
int ddcl();
void parse();
int paraparse();
int pdcl();
int ppdcl();

int get_token_type()
{
  char *p = g_token;
  int c = getch();
  while (c == ' ' || c == '\t')
    c = getch();
  if (c == '(') {
    if (g_paren_type == 1) {
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
    g_paren_type = 1;
  }
  else {
    g_token_type = c;
  }
  *p = '\0';
  return g_token_type;
}

int dcl()
{
  g_paren_type = 0;
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

int pdcl()
{
  g_paren_type = 0;
  int res;
  int ns = 0;
  while (get_token_type() == '*') {
    ++ns;
  }
  if ((res = ddcl()) != OK)
    return res;
  while (ns-- > 0)
    strcat(g_par_dcl, "pointer to ");
  return res;
}

int pddcl()
{
  if (g_token_type == NAME) {
    strcpy(g_par_name, g_token);
  }
  else if (g_token_type == '(') {
    int res;
    if ((res = pdcl()) != OK)
      return res;
    if (g_token_type != ')')
      return ERR_MISSING_CLOSING_PARENTHESIS;
  }
  else {
    return ERR_EXPECT_NAME_OR_DCL_IN_PARENTHESES;
  }
  for (int tt = get_token_type(); tt == PARENTHESES || tt == BRACKETS;
       tt = get_token_type()) {
    if (tt == PARENTHESES) {
      int paratt = get_token_type();
      if (g_token_type == ')')
        strcat(g_par_dcl, "function of void returning ");
      else
        return ERR_MISSING_CLOSING_PARENTHESIS;
//       else {
//         strcat(g_par_dcl, "function of ");
// SPAGHETTI:
//         if ((res = paraparse()) != OK)
//           return res;
//         strcat(g_par_dcl, g_par_dcl);
//         strcat(g_par_dcl, g_par_type);
//         get_token_type();
//         if (g_token_type == ',') {
//           strcat(g_par_dcl, " and ");
//           get_token_type();
//           goto SPAGHETTI;
//         }
//         if (g_token_type != ')')
//           return ERR_MISSING_CLOSING_PARENTHESIS;
//         strcat(g_par_dcl, " returning ");
//       }
    }
    else {
      strcat(g_par_dcl, "array");
      strcat(g_par_dcl, g_token);
      strcat(g_par_dcl, " of ");
    }
  }
  return OK;
}

int ddcl()
{
  int res;
  if (g_token_type == NAME) {
    strcpy(g_name, g_token);
  }
  else if (g_token_type == '(') {
    if ((res = dcl()) != OK)
      return res;
    if (g_token_type != ')')
      return ERR_MISSING_CLOSING_PARENTHESIS;
  }
  else {
    return ERR_EXPECT_NAME_OR_DCL_IN_PARENTHESES;
  }
  for (int tt = get_token_type(); tt == PARENTHESES || tt == BRACKETS;
       tt = get_token_type()) {
    if (tt == PARENTHESES) {
      int paratt = get_token_type();
      if (g_token_type == ')')
        strcat(g_dcl, "function of void returning ");
      else {
        strcat(g_dcl, "function of ");
SPAGHETTI:
        if ((res = paraparse()) != OK)
          return res;
        strcat(g_dcl, g_par_dcl);
        strcat(g_dcl, g_par_type);
        get_token_type();
        if (g_token_type == ',') {
          strcat(g_dcl, " and ");
          get_token_type();
          goto SPAGHETTI;
        }
        if (g_token_type != ')')
          return ERR_MISSING_CLOSING_PARENTHESIS;
        strcat(g_dcl, " returning ");
      }
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
      printf("[error] syntax error\n");
      break;
    default:
      printf("[error] unknown error\n");
      break;
  }
  fprintf(stderr,
          "[error] g_name = %s, g_dcl = %s, g_type = %s, g_token = %s, "
          "g_token_type = %d\n",
          g_name, g_dcl, g_type, g_token, g_token_type);
  // ignore all trailing characters in the line
  if (g_token_type != '\n') {
    while (getch() != '\n')
      ;
  }
}

void parse()
{
  strcpy(g_type, g_token);
  g_dcl[0] = '\0';
  int res = dcl();
  if (res == OK && g_token_type != '\n')
    res = ERR_SYNTAX_ERROR;
  if (res == OK)
    printf("%s:  %s%s\n", g_name, g_dcl, g_type);
  else
    error_handle(res);
}

int paraparse()
{
  strcpy(g_par_type, g_token);
  g_par_dcl[0] = '\0';
  int res = pdcl();
  if (res == OK && g_token_type != '\n')
    res = ERR_SYNTAX_ERROR;
  return res;
}

void clean()
{
  g_name[0] = g_type[0] = g_dcl[0] = '\0';
}

int main()
{
  while (get_token_type() != EOF) {
    parse();
    clean();
  }
  return 0;
}
