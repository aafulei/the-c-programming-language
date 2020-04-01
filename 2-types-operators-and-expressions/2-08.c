/* Exercise 2-8. Write a function rightrot(x,n) that returns the value of the
   integer x rotated to the right by n positions. */

#include <stdio.h>

unsigned rightrot(unsigned x, int n);
int bits(void);

/* right rotate an integer (0x95e2) by 32 times */
int main(void)
{
    unsigned x;
    int i;

    x = 0x95e2;
    for (i = 1; i <= 32; i++)
        printf("0x%x right rotated by %d is 0x%x\n", x, i, rightrot(x, i));
    return 0;
}

/* return x with its value right rotated by n positions */
unsigned rightrot(unsigned x, int n)
{
    int b;

    b = bits();
    n = n % b;
    return x >> n | (x & ~(~0 << n)) << (b-n);
}

/* get number of bits for unsigned type on local machine */
int bits(void)
{
    unsigned x;
    int i;

    x = ~0;
    for (i = 0; x != 0; i++)
        x = x << 8;
    return i * 8;
}
