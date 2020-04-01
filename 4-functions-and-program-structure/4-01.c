/* Exercise 4-1. Write the function strrindex(s,t) which returns the position
   of the rightmost occurrence of t in s, or -1 if there is none. */

#include <stdio.h>

int strrindex(char s[], char t[]);
int strlength(char s[]);

/* test */
int main(void)
{
    printf("%d\n", strrindex("abcdbcd", ""));       /* -1 */
    printf("%d\n", strrindex("abcdbcd", "x"));      /* -1 */
    printf("%d\n", strrindex("abcdbcd", "d"));      /* 6 */
    printf("%d\n", strrindex("abcdbcd", "bc"));     /* 4 */
    printf("%d\n", strrindex("abcdbcd", "abc"));    /* 0 */
    printf("%d\n", strrindex("abcdbcd", "bcbc"));   /* -1 */
    return 0;
}

/* return index of rightmost occurrence of t in s, -1 if none */
int strrindex(char s[], char t[])
{
    int i, j, k, m, n;

    m = strlength(s);
    if ((n = strlength(t)) == 0)    /* corner case: empty t */
        return -1;
    for (i = m - n; i >= 0; i--) {
        for (j = i, k = 0; k < n && s[j] == t[k]; j++, k++)
            ;
        if (k == n)
            return i;
    }
    return -1;
}

/* return length of s */
int strlength(char s[])
{
    int i;

    for (i = 0; s[i] != '\0'; i++)
        ;
    return i;
}
