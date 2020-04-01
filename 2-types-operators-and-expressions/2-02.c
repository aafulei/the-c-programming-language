/* Exercise 2-2. Write a loop equivalent to the for loop above without using &&
   or ||. */

/* The original for loop:
    for (i=0; i < lim-1 && (c=getchar()) != '\n' && c != EOF; ++i)
        s[i] = c; */

/* See Section 1.9 (page 29) for the code below */

#include <stdio.h>
#define MAXLINE 1000  /* maxium input line size */

int getline(char line[], int maxline);

/* copy input to output, line by line */
int main()
{
    char line[MAXLINE]; /* current input line */

    while (getline(line, MAXLINE) > 0)
        printf("%s", line);
    return 0;
}

/* getline: read a line into s, return length */
int getline(char s[], int lim)
{
    int c, i;
    enum cond {NO, YES} ok;

    i = 0;
    ok = YES;
    /* equivalent loop begins */
    while (ok == YES) {
        if (i >= lim-1)
            ok = NO;
        else if ((c = getchar()) == '\n')
            ok = NO;
        else if (c == EOF)
            ok = NO;
        else {
            s[i] = c;
            ++i;
        }
    }
     /* equivalent loop ends */
    if (c == '\n') {
        s[i] = c;
        ++i;
    }
    s[i] = '\0';
    return i;
}

/* Note: Enumeration has just been introduced. We intentionally use it here for
   a quick review. `break` and postfix increment have not been introduced yet;
   otherwise they can be used here. */
