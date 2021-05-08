/*  Exercise 5-6. Rewrite appropriate programs from earlier chapters and
   exercises with pointers instead of array indexing. Good possibilities
   include getline (Chapters 1 and 4), atoi, itoa, and their variants
   (Chapters  2, 3, and 4), reverse (Chapter 3), and strindex and getop
   (Chapter 4). */

#include <ctype.h>
#include <stdio.h>
#include <math.h>

#define ABS(x) (x > 0 ? x : -(x))

int getline(char *p, int lim)
{
    int c;
    char *b = p;

    while (p-b < lim-1 && (c=getchar()) != EOF && c != '\n')
        *p++ = c;
    if (c == '\n')
        *p++ = c;
    *p++ = '\0';
    return p-b;
}

/*  0. leading white spaces ignored
    1. optional + or - sign
    2. leading zeros ignored (e.g. 000123 is 123)
    3. overflow has undefined behavior
    4. if no digits, return 0 */
int atoi(char *p)
{
    int i, sign;

    while (isspace(*p))
        ++p;
    sign = 1;
    if (*p == '+')
        p++;
    else if (*p == '-') {
        sign = -1;
        p++;
    }
    for (i = 0; isdigit(*p); p++)
        i = 10 * i - (*p - '0');    /* don't omit parentheses, may overflow */
    return sign == 1 ? -i : i;      /* start with negative, for -2147483648 */
}

void reverse(char *p)
{
    char *a, t;

    a = p;
    while (*p != '\0')
        ++p;
    if (a == p)
        return;
    for (--p; a < p; a++, p--)
        t = *p, *p = *a, *a = t;
}

/* See Exercises 3-04, 4-12 */
/* quotient has more information than remainder */
/* but it's hard for ANSI to write in implementation-independent way to use quotient */
char *itoa2(int i, char *p)
{
    char *a;
    int q, neg;

    a = p;
    // while (q = (i / 10)) {
    //     *p++ = i - q * 10 + '0'; /* common pitfall: forget '0' */
    //     i = q;
    // }
    // *p++ = i - q * 10 + '0';
    // *p = '\0';

    // do {
    //     *p++ = i - (q = i / 10) * 10 + '0';
    // } while (i = q);

    // do {
    //     *p++ = i % 10 + '0';
    // } while (i /= 10);

    neg = i < 0;
    do {
        *p++ = abs(i - (q = i / 10) * 10) + '0';    /* pitfall for ABS as macro: -x not -(x) -- x could an expression*/
    } while (i = q);
    if (neg)
        *p++ = '-';
    *p = '\0';
    reverse(a);
    return a;
}

char *fill(int i, char *p)
{
    int q;

    if (q = i / 10) {
        p = fill(q, p);
        *p = abs(i - q * 10) + '0';
    }
    else {
        if (i >= 0)
            *p = i + '0';
        else {
            *p++ = '-';
            *p = -i + '0';
        }
    }
    return p+1;
}

char *itoa(int i, char *p)
{
    *fill(i, p) = '\0';
    return p;
}

int main(void)
{
    char a[100];

    printf("%d\n", atoi("0"));
    printf("%d\n", atoi("-0"));
    printf("%d\n", atoi("123"));
    printf("%d\n", atoi("-456"));
    printf("%d\n", atoi("+789xyz"));
    printf("%d\n", atoi("+2147483647"));
    printf("%d\n", atoi("-000002147483648"));
    printf("%s\n", itoa(0, a));
    printf("%s\n", itoa(1, a));
    printf("%s\n", itoa(9, a));
    printf("%s\n", itoa(10, a));
    printf("%s\n", itoa(123, a));
    printf("%s\n", itoa(456, a));
    printf("%s\n", itoa(2147483647, a));
    printf("%s\n", itoa(-1, a));
    printf("%s\n", itoa(-9, a));
    printf("%s\n", itoa(-10, a));
    printf("%s\n", itoa(-789, a));
    printf("%s\n", itoa(-2147483648, a));
    return 0;
}
