/* Exercise 1-17. Write a program to print all input lines that are longer than
   80 characters. */

#include <stdio.h>

#define MAXLINE 1000
#define MINLINE 80

int getline(char line[], int maxline);

/* print input lines that are longer than 80 characters */
int main(void)
{
    int len;
    char line[MAXLINE];

    len = 0;
    while ((len = getline(line, MAXLINE)) > 0)
        if (len > MINLINE)
            printf("%s", line);
    return 0;
}

/* modified getline: read a line into s, return original length */
int getline(char s[], int lim)
{
    int c, i;

    for (i = 0; (c = getchar()) != EOF && c != '\n'; ++i)
        if (i < lim-1)
            s[i] = c;
    if (c == '\n') {
        if (i < lim-1)
            s[i] = c;
        ++i;
    }
    if (i < lim)
        s[i] = '\0';
    else
        s[lim-1] = '\0';
    return i;
}

/* Note: `break` and postfix increment have not been introduced yet; otherwise
   they can be used here. */
