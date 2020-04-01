/* Exercise 3-5. Write the function itob(n,s,b) that converts the integer n
   into a base b character representation in the string s. In particular,
   itob(n,s,16) formats s as a hexadecimal integer in s. */

#include <stdio.h>

#define MAX 100

void itob(int n, char s[], int b);
int digit(int x);
void reverse(char s[]);

/* print 100 in bases from 2 to 20 */
int main(void)
{
    char s[MAX];
    int n, b;

    n = 100;
    for (b = 2; b <= 20; b++) {
        itob(n, s, b);
        printf("%d in base %d is %s\n", n, b, s);
    }
    return 0;
}

/* convert integer n to characters s in base b */
void itob(int n, char s[], int b)
{
    int i, neg;

    neg = n < 0;
    i = 0;
    do
        s[i++] = digit(n % b);
    while ((n /= b) != 0);
    if (neg)
        s[i++] = '-';
    s[i++] = '\0';
    reverse(s);
}

/* return digit representation of x */
int digit(int x)
{
    if (x < 0)
        x = -x;
    if (x < 10)
        return x + '0';
    if (x < 36)
        return x - 10 + 'a';
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

/* Note: It is users' responsibility to ensure a proper range for base. */
