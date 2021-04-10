#include <stdio.h>
#include <ctype.h>
#include <string.h>

char g_name[100];
char g_token[100];
char g_out[1000];
int g_last_token_type;

void dcl(void)
{
    int stars = 0;
    while (1) {
        char t = get_token();
        if (t == '*')
            ++stars;
        else
            break;
    }
    ddcl();
    while (stars-- > 0)
        strcat(g_out, " pointer to");
}

void ddcl(void)
{
    int type;
    if (g_last_token_type == '(') {
        dcl();
        if (g_last_token_type != ')')
            printf("missing )!\n");
    }
    else if (g_last_token_type == g_name) {
        strcpy(g_name, g_token);
    }
    else
        printf("error: expect name or (dcl)\n");
    while (1) {
        type = get_token();
        if (type == PARENS) {
            strcat(g_out, " function returning");
        }
        else if (type == BRACKETS) {
            strcat(g_out, " array");
            strcat(g_out, g_token);
            strcat(g_out, " of");
        }
        else
            break;
    }
}


int main()
{
    // while (get_token() != EOF) {

    // }
    return 0;
}
