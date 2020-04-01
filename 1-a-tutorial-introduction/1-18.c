/* Exercise 1-18. Write a program to remove trailing blanks and tabs from each
   line of input, and to delete entirely blank lines. */

#include <stdio.h>

#define MAXLINE 1000    /* maxium input line size */

int getline(char line[], int maxline);

/* print input lines, removing trailing blanks and tabs */
int main()
{
    char line[MAXLINE];

    while (getline(line, MAXLINE) > 0)  /* get until original line is empty */
        if (line[0] != '\0')    /* skip lines that are blank after removal */
            printf("%s", line);
    return 0;
}

/* modified getline: read a line into s, remove trailing blanks and tabs,
   return original length */
int getline(char s[], int lim)
{
    int c, i, lasti;

    lasti = -1; /* last position of a non-blank character */
    for (i = 0; (c=getchar()) != EOF && c != '\n'; ++i) {
        if (i < lim-1) {
            if (c != ' ' && c != '\t')
                lasti = i;
            s[i] = c;
        }
    }
    if (c == '\n') {
        if (lasti >= 0) {
            ++lasti;
            s[lasti] = c;
        }
        ++i;
    }
    s[lasti+1] = '\0';
    return i;
}

/* Note: There seems to be two different ways to interpret the question:
   1. For each line of input, remove trailing blanks and tabs in the process of
      getting the line;
   2. For each line of input, first get the line and store it in a char array,
      and then remove trailing blanks and tabs in the char array.
   We adopt the first way.
   Besides, what if asked to remove leading blanks and tabs? Consider
   while ((c=getchar()) != EOF && (c == ' ' || c == '\t'))
       ;
   (The code that follows needs to be adjusted accordingly.) */
