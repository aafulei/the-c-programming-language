/* Exercise 3-3. Write a function expand(s1,s2) that expands shorthand
   notations like a-z in the string s1 into the equivalent complete list
   abc...xyz in s2. Allow for letters of either case and digits, and be
   prepared to handle cases like a-b-c and a-z0-9 and -a-z. Arrange that a
   leading or trailing - is taken literally. */

#include <stdio.h>

#define MAXLINE 1000

void expand(char s1[], char s2[]);
int getline(char s[], int lim);

int main(void)
{
    char s1[MAXLINE];
    char s2[MAXLINE];

    while (getline(s1, MAXLINE) > 0) {
        expand(s1, s2);
        printf("%s", s2);
    }
    return 0;
}

void expand(char s1[], char s2[])
{
    int i, j, lastc;

    lastc = '\0';
    for (i = j = 0; s1[i] != '\0'; i++) {
        if (s1[i] == '-' &&
            ('a'<=lastc && lastc<='z' && 'a'<=s1[i+1] && s1[i+1]<='z' ||
             'A'<=lastc && lastc<='Z' && 'A'<=s1[i+1] && s1[i+1]<='Z' ||
             '0'<=lastc && lastc<='9' && '0'<=s1[i+1] && s1[i+1]<='9')) {
            if (lastc < s1[++i])
                while (lastc != s1[i])
                    s2[j++] = ++lastc;
            else
                while (lastc != s1[i])
                    s2[j++] = --lastc;
        }
        else
            s2[j++] = s1[i];
        lastc = s1[i];
    }
    s2[j] = '\0';
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

/* Note: It is users' responsibility in this case not to overflow the array. */
