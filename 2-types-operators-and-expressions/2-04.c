/* Exercise 2-4. Write an alternative version of squeeze(s1,s2) that deletes
   each character in s1 that matches any character in the string s2. */

/* See Section 2.8 (page 47) for the original squeeze().
    void squeeze(char s[], int c)
    {
        int i, j;
        for (i = j = 0; s[i] != '\0'; i++)
            if (s[i] != c)
                s[j++] = s[i];
        s[j] = '\0';
    } */

#include <stdio.h>

#define MAXLINE 1000

void squeeze(char s1[], char s2[]);
int getline(char s[], int lim);

/* get a line, remove vowels */
int main(void)
{
    char s1[MAXLINE];
    char s2[11] = "aeiouAEIOU";

    while (getline(s1, MAXLINE) > 0) {
        squeeze(s1, s2);
        printf("%s", s1);
    }

    return 0;
}

/* delete each character in s1 that matches any character in s2 */
void squeeze(char s1[], char s2[])
{
    int i, j, k;

    for (i = j = 0; s1[i] != '\0'; i++) {
        /* `break` or a `match` indicator is not necessary here */
        for (k = 0; s2[k] != '\0' && s2[k] != s1[i]; k++)
            ;
        if (s2[k] == '\0')
            s1[j++] = s1[i];
    }
    s1[j] = '\0';
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
