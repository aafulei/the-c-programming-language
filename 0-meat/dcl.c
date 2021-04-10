// 21/04/10 = Sat

// gcc dcl.c getch.c

// Exercise 5-18. Make dcl recover from input errors.


#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define TOKEN_SIZE 100
#define DERIVED_SIZE 1000
#define OK 0
#define ERR_MISSING_CLOSING_PARENTHESIS 1
#define ERR_EXPECT_NAME_OR_DCL_IN_PARENTHESES 2
#define ERR_SYNTAX_ERROR 3

int dcl(void);
int ddcl(void);
int  get_token_type(void);
int  getch();
int  ungetch(char);

char g_name[TOKEN_SIZE];
char g_primitive[TOKEN_SIZE];
char g_derived[DERIVED_SIZE];
char g_last_token[TOKEN_SIZE];
int  g_last_token_type;
enum { NAME, PARENTHESES, BRACKETS };

int dcl(void)
{
    int num_stars = 0;
    while (get_token_type() == '*')
        ++num_stars;
    int ret;
    if ((ret = ddcl()) != OK)
        return ret;
    while (num_stars-- > 0)
        strcat(g_derived, " pointer to");
    return OK;
}

int ddcl(void)
{
    if (g_last_token_type == '(') {
        int ret;
        if ((ret = dcl()) != OK)
            return ret;
        if (g_last_token_type != ')') {
            return ERR_MISSING_CLOSING_PARENTHESIS;
        }
    }
    else if (g_last_token_type == NAME) {
        strcpy(g_name, g_last_token);
    }
    else {
        return ERR_EXPECT_NAME_OR_DCL_IN_PARENTHESES;
    }
    while (1) {
        get_token_type();
        if (g_last_token_type == PARENTHESES) {
            strcat(g_derived, " function returning");
        }
        else if (g_last_token_type == BRACKETS) {
            strcat(g_derived, " array");
            strcat(g_derived, g_last_token);
            strcat(g_derived, " of");
        }
        else
            break;
    }
    return OK;
}

int get_token_type(void)
{
    char   c;
    char * p = g_last_token;
    do
        c = getch();
    while (c == ' ' || c == '\t');
    if (c == '(') {
        if ((c = getch()) == ')') {
            strcpy(g_last_token, "()");
            g_last_token_type = PARENTHESES;
        }
        else {
            ungetch(c);
            g_last_token_type = '(';
        }
    }
    else if (c == '[') {
        for (*p++ = c; (*p++ = getch()) != ']'; )
            ;
        *p = '\0';
        g_last_token_type = BRACKETS;
    }
    else if (isalpha(c)) {
        for (*p++ = c; isalnum(c = getch()); *p++ = c)
            ;
        *p = '\0';
        ungetch(c);
        g_last_token_type = NAME;
    }
    else {
        g_last_token_type = c;
    }
    return g_last_token_type;
}

void error_handle(int ret)
{
    switch (ret) {
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
    // eat all remaining characters in the line
    if (g_last_token_type != '\n') {
        while (getch() != '\n')
            ;
    }
}


int main()
{
    while (get_token_type() != EOF) {
        strcpy(g_primitive, g_last_token);
        g_derived[0] = '\0';
        int ret = dcl();
        if (ret == OK && g_last_token_type != '\n')
            ret = ERR_SYNTAX_ERROR;
        if (ret == OK)
            printf("%s: %s %s\n", g_name, g_derived, g_primitive);
        else
            error_handle(ret);
    }
    return 0;
}
