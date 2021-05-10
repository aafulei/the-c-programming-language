// work for a lot of cases
// todo: void foo(int (*bar)(long))

/* Exercise 5-20. Expand dcl to handle declarations with function argument
 * types, qualifiers like const, and so on. */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAXSIZE 1000
#define BUFSIZE 100

static char g_buffer[BUFSIZE];
static char *g_next = g_buffer;

char g_token[100];


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

#define OK                                        0
#define ERR_MISSING_CLOSING_PARENTHESIS           1
#define ERR_EXPECT_NAME_OR_DCL_IN_PARENTHESES     2
#define ERR_SYNTAX_ERROR                          3
#define ERR_PAR_MISSING_CLOSING_PARENTHESIS       4
#define ERR_PAR_EXPECT_NAME_OR_DCL_IN_PARENTHESES 5
#define ERR_PAR_SYNTAX_ERROR                      6

enum { NAME, PARENTHESES, BRACKETS };
int g_token_type;
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
int pddcl();

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
  if ((res = pddcl()) != OK)
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
      return ERR_PAR_MISSING_CLOSING_PARENTHESIS;
  }
  else if (g_token_type == ',' || g_token_type == ')') {
    return OK;
  }
  else {
    return ERR_PAR_EXPECT_NAME_OR_DCL_IN_PARENTHESES;
  }
  for (int tt = get_token_type(); tt == PARENTHESES || tt == BRACKETS;
       tt = get_token_type()) {
    if (tt == PARENTHESES) {
      int paratt = get_token_type();
      if (g_token_type == ')')
        strcat(g_par_dcl, "function of void returning ");
      else
        return ERR_PAR_MISSING_CLOSING_PARENTHESIS;
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
        if (strlen(g_par_name)) {
          strcat(g_dcl, " called ");
          strcat(g_dcl, g_par_name);
        }
        // get_token_type();
        if (g_token_type == ',') {
          strcat(g_dcl, " and ");
          get_token_type();
          goto SPAGHETTI;
        }
        // if (g_token_type != ')')
        //   return ERR_MISSING_CLOSING_PARENTHESIS;
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
    case ERR_PAR_MISSING_CLOSING_PARENTHESIS:
      printf("[error] para missing closing parenthesis ')'\n");
      break;
    case ERR_PAR_EXPECT_NAME_OR_DCL_IN_PARENTHESES:
      printf("[error] para expect name or (dcl)\n");
      break;
    case ERR_PAR_SYNTAX_ERROR:
      printf("[error] para syntax error\n");
      break;
    default:
      printf("[error] unknown error\n");
      break;
  }
  fprintf(
      stderr,
      "[error] g_name = %s, g_dcl = %s, g_type = %s, g_token = %s, "
      "g_token_type = %d, g_par_name = %s, g_par_dcl = %s, g_par_type = %s, g_paren_type = %d\n",
      g_name, g_dcl, g_type, g_token, g_token_type, g_par_name, g_par_dcl,
      g_par_type, g_paren_type);
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
  g_par_name[0] = '\0';
  int res = pdcl();
  if (res == OK && g_token_type != ')' && g_token_type != ',')
    res = ERR_PAR_SYNTAX_ERROR;
  g_paren_type = 1;
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
