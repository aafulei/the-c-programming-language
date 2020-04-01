/* Exercise 4-12. Adapt the ideas of printd to write a recursive version of
   itoa; that is, convert an integer into a string by calling a recursive
   routine. */

#include <stdio.h>

#define MAX 100

void itoa(int, char []);
void fill(int, char [], int);
void reverse(char []);
int abs(int);

/* test */
int main(void)
{
    int i;
    char a[MAX];

    itoa(-1234, a);
    printf("%s\n", a);  /* -1234 */
    return 0;
}

/* convert an integer i into a string a */
void itoa(int i, char a[])
{
    fill(i, a, 0);
    reverse(a);
}

/* recursively fill char array with digits of integer i */
void fill(int i, char a[], int j)
{
    int q;

    q = i / 10;
    a[j] = abs(i - q * 10) + '0';
    if (q)
        fill(q, a, j+1);
    else {
        if (i < 0)
            a[++j] = '-';
        a[j+1] = '\0';
    }
}

/* reverse string a */
void reverse(char a[])
{
    int i, j, t;

    for (j = 0; a[j] != '\0'; j++)
        ;
    /* Don't forget --j; otherwise you always get an "empty" string! */
    --j;
    for (i = 0; i < j; i++, j--)
        t = a[i], a[i] = a[j], a[j] = t;
}

/* return absolute value of i */
int abs(int i)
{
    return i >= 0 ? i : -i;
}
