/* Exercise 3-6. Write a version of itoa that accepts three arguments instead
   of two. The third argument is a minimum field width; the converted number
   must be padded with blanks on the left if necessary to make it wide
   enough. */

#include <limits.h>
#include <stdio.h>

#define MAX 100

void itoa(int n, char s[], int w);
int abs(int x);
void reverse(char s[]);

/* print the largest negative number twice, with min field widths of 5, 50 */
int main(void)
{
    char s[MAX];

    itoa(INT_MIN, s, 5);
    printf("%s\n", s);
    itoa(INT_MIN, s, 50);
    printf("%s\n", s);
    return 0;
}

/* padded itoa: convert integer to characters, respecting a min field width */
void itoa(int n, char s[], int w)
{
    int i, neg;

    neg = n < 0;
    i = 0;
    do
        s[i++] = abs(n % 10) + '0';
    while ((n /= 10) != 0);
    if (neg)
        s[i++] = '-';
    while (i < w)
        s[i++] = ' ';
    s[i] = '\0';
    reverse(s);
}

/* return absolute value of x */
int abs(int x)
{
    return x >= 0 ? x : -x;
}

/* reverse a char array */
void reverse(char s[])
{
    int i, j, t;

    for (i = 0; s[i] != '\0'; i++)
        ;
    for (i--, j = 0; j < i; j++, i--)
        t = s[j], s[j] = s[i], s[i] = t;
}
