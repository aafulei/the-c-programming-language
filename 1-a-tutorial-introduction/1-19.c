/* Exercise 1-19. Write a function reverse(s) that reverses the character
   string s. Use it to write a program that reverses its input a line at a
   time. */

#include <stdio.h>

#define MAXLINE 10

int getline(char line[], int maxline);
void reverse(char line[]);

/* reverse and print input lines */
int main(void)
{
    char line[MAXLINE];

    while (getline(line, MAXLINE) > 0) {
        reverse(line);
        printf("%s", line);
    }
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

/* reverse string */
void reverse(char s[])
{
    int i, j;
    char t;

    /* get string length of s */
    i = 0;
    while (s[i] != '\0')
        ++i;
    /* nothing to do if s is empty */
    if (i == 0)
        return;
    /* back off from '\0' and ignore newline (if any) */
    --i;
    if (s[i] == '\n')
        --i;
    /* swap characters */
    j = 0;
    while (j < i) {
        t = s[j];
        s[j] = s[i];
        s[i] = t;
        ++j;
        --i;
    }
}

/* Note: Though it is never possible for `reverse` to work on an empty string
in this case, we still check against so. In general, we need to be sure not to
access an array out of bounds. Besies, comma operator has not been introduced
yet; otherwise, it can be used here. */
