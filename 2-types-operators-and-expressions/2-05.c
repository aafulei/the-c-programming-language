/* Exercise 2-5. Write the function any(s1,s2), which returns the first
   location in a string s1 where any character from the string s2 occurs, or -1
   if s1 contains no characters from s2. (The standard library function strpbrk
   does the same job but returns a pointer to the location.) */

#include <stdio.h>

#define MAXLINE 1000

int any(char s1[], char s2[]);
int getline(char s[], int lim);

/* get two lines from input, return the first index in line1 where any
   character from line2 occurs; if no, return -1 */
int main(void)
{
    char line1[MAXLINE];
    char line2[MAXLINE];

    while (getline(line1, MAXLINE) > 0) {
        getline(line2, MAXLINE);
        printf("%d\n", any(line1, line2));
    }
    return 0;
}

/* return first index in s1 where any character from s2 occurs, -1 if none */
int any(char s1[], char s2[])
{
    int i, j;

    for (i = 0; s1[i] != '\0'; i++)
        for (j = 0; s2[j] != '\0'; j++)
            if (s2[j] == s1[i])
                return i;
    return -1;
}

/* simple getline: read a line, ignoring newline, return length */
int getline(char s[], int lim)
{
    int c, i;

    for (i = 0; (c = getchar()) != EOF && c != '\n'; i++)
        if (i < lim - 1)
            s[i] = c;
    if (i < lim)
        s[i] = '\0';
    else
        s[lim-1] = '\0';
    return i;
}
