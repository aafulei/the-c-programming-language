/* Exercise 1-16. Revise the main routine of the longest-line program so it
   will correctly print the length of arbitrary long input lines, and as much
   as possible of the text. */

/* See Section 1.9 (page 29) for the original program */

#include <stdio.h>

#define MAXLINE 10    /* maximum input line length */

int getline(char line[], int maxline);
void copy(char to[], char from[]);

/* print the lengths (and as much as possible the text) of the input lines, and
   print the longest input line */
int main(void)
{
    int len;            /* current line length */
    int max;            /* maximum length seen so far */
    char line[MAXLINE];     /* current input line */
    char longest[MAXLINE];  /* longest line saved here */

    max = 0;
    while ((len = getline(line, MAXLINE)) > 0) {
        printf("[%d] %s", len, line);   /* modified: print length and text */
        if (len > max) {
            max = len;
            copy(longest, line);
        }
    }
    if (max > 0)    /* there was a line */
        printf("%s", longest);
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

/* copy: copy 'from' into 'to'; assume to is big enough */
void copy(char to[], char from[])
{
   int i;

   i = 0;
   while ((to[i] = from[i]) != '\0')
       ++i;
}

/* Note: The logic is simple. Given a char array of length n,
   1. read input until an EOF or a newline;
   2. for any character (including newline), increment length counter by 1, and
      write to the first (n-1) of the char array if there is room;
   3. store the null character at the end of the char array.
   Besides, `break` and postfix increment have not been introduced yet;
   otherwise they can be used here. */
