/* Exercise 3-4. In a two's complement number representation, our version of
   itoa does not handle the largest negative number, that is, the value of n
   equal to -(2^(wordsize-1)). Explain why not. Modify it to print that value
   correctly, regardless of the machine on which it runs. */

/* See Section 3.6 (page 64) for the original itoa
    void itoa(int n, char s[])
    {
        int i, sign;

        if ((sign = n) < 0)
            n = -n;
        i = 0;
        do {
            s[i++] = n % 10 + '0';
        } while ((n /= 10) > 0);
        if (sign < 0)
            s[i++] = '-';
        s[i] = '\0';
        reverse(s);
    } */

/* Answer: In a two's complement system, a negative number (-x) is represented
   by inverting the corresponding positive number x and then adding by 1. For
   example, to get -1 for a 32-bit integer, we invert 1 (0x0001) to get
   0xfffe and then add by 1 to get 0xffff. In such a system, the largest
   negative number has its absolute value greater than the largest possible
   positive number by 1. Thus, if n is the largest negative number, n = -n
   would cause an overflow, which has undefined behavior. */

#include <limits.h>
#include <stdio.h>

#define MAX 100

void itoa(int n, char s[]);
int abs(int x);
void reverse(char s[]);

/* convert the largest negative number to char array and print it out */
int main(void)
{
    char s[MAX];

    itoa(INT_MIN, s);
    printf("%s\n", s);
    return 0;
}

/* modified itoa: convert integer to characters */
void itoa(int n, char s[])
{
    int i, neg;

    neg = n < 0;
    i = 0;
    do
        s[i++] = abs(n % 10) + '0';
    while ((n /= 10) != 0);
    if (neg)
        s[i++] = '-';
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

/* Note: If you want to #define ABS(x), make sure it's -(x), not (-x)!
   In ANSI C, it is possible that (-9) / 10 = (-1) ... 1. However, it has been
   fixed since C99 that (-9) / 10 = 0 ... (-9). Here we rule out the ambiguity
   by adopting the new standard that fixes the result. The code that works for
   both implementations as allowed by ANSI C is unnecessarily complicated and
   meaningless. */
