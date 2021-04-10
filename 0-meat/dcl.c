// gcc dcl.c getch.c

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define TOKEN_SIZE 100
#define DERIVED_SIZE 1000

void dcl(void);
void ddcl(void);
int  get_token_type(void);
int  getch();
int  ungetch(char);

char g_name[TOKEN_SIZE];
char g_primitive[TOKEN_SIZE];
char g_derived[DERIVED_SIZE];
char g_last_token[TOKEN_SIZE];
int  g_last_token_type;
enum { NAME, PARENTHESES, BRACKETS };

void dcl(void)
{
    int num_stars = 0;
    while (get_token_type() == '*')
        ++num_stars;
    ddcl();
    while (num_stars-- > 0)
        strcat(g_derived, " pointer to");
}

void ddcl(void)
{
    if (g_last_token_type == '(') {
        dcl();
        if (g_last_token_type != ')')
            printf("ddcl(): [error] missing )\n");
    }
    else if (g_last_token_type == NAME) {
        strcpy(g_name, g_last_token);
    }
    else {
        printf("ddcl(): [error] expect name or (dcl)\n");
    }
    while (1) {
        int token_type = get_token_type();
        if (token_type == PARENTHESES) {
            strcat(g_derived, " function returning");
        }
        else if (token_type == BRACKETS) {
            strcat(g_derived, " array");
            strcat(g_derived, g_last_token);
            strcat(g_derived, " of");
        }
        else
            break;
    }
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

int main()
{
    while (get_token_type() != EOF) {
        strcpy(g_primitive, g_last_token);
        g_derived[0] = '\0';
        dcl();
        if (g_last_token_type != '\n') {
            printf("main(): [error] syntax error\n");
        }
        printf("%s: %s %s\n", g_name, g_derived, g_primitive);
    }
    return 0;
}
