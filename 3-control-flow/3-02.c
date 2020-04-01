/* Exercise 3-2. Write a function escape(s,t) that converts characters like
   newline and tab into visible escape sequences like \n and \t as it copies
   the string t to s. Use a switch. Write a function for the other direction as
   well, converting escape sequences into the real characters. */

#include <stdio.h>

#define MAXLINE 1000

int getline(char s[], int lim);
void escape(char s[], char t[]);
void real(char s[], char t[]);

int main(void)
{
    char s[MAXLINE * 2];
    char t[MAXLINE];

    while (getline(t, MAXLINE) > 0) {
        escape(s, t);
        printf("%s\n", s);
        real(t, s);
        printf("%s", t);
    }
    return 0;
}

/* modified getline: read a line into s, return original length */
int getline(char s[], int lim)
{
    int c, i;

    for (i = 0; (c = getchar()) != EOF && c != '\n'; i++)
        if (i < lim - 1)
            s[i] = c;
    if (c == '\n') {
        if (i < lim - 1)
            s[i] = c;
        i++;
    }
    if (i < lim)
        s[i] = '\0';
    else
        s[lim-1] = '\0';
    return i;
}

/* copy from s to t, replace newlines with \n and tabs with \t */
void escape(char s[], char t[])
{
    int i, j;

    for (i = j = 0; t[i] != '\0'; i++)
        switch (t[i]) {
            case '\n': s[j++] = '\\'; s[j++] = 'n'; break;
            case '\t': s[j++] = '\\'; s[j++] = 't'; break;
            default: s[j++] = t[i];
        }
    s[j] = '\0';
}

/* copy from s to t, replace \n with newlines and \t with tabs */
void real(char s[], char t[])
{
    int i, j;

    for (i = j = 0; t[i] != '\0'; i++)
        if (t[i] == '\\')
            switch (t[++i]) {
                case 'n': s[j++] = '\n'; break;
                case 't': s[j++] = '\t'; break;
                default: s[j++] = '\\'; s[j++] = t[i];
            }
        else
            s[j++] = t[i];
    s[j] = '\0';
}
