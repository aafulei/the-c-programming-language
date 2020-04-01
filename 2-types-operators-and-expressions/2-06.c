/* Exercise 2-6. Write a function setbits(x,p,n,y) that returns x with the n
   bits that begin at position p set to the rightmost n bits of y, leaving the
   other bits unchanged. */

#include <stdio.h>

unsigned setbits(unsigned x, int p, int n, unsigned y);

/* Let x=0x95e2, p=5, n=3, y=0x9.
Convert 3 bits of x (100) to 3 bits from y (001)
pos    [FEDC BA98 7654 3210]
[x]
bin     1001 0101 1110 0010
hex        9    5    e    2
mask                ** *
[y]
bin     0000 0000 0000 1001
hex        0    0    0    9
mask                    ***
[r]
bin     1001 0000 1100 1010
hex        9    5    c    a
*/
int main(void)
{
    unsigned x, y;
    int p, n;

    x = 0x95e2;
    p = 5;
    n = 3;
    y = 0x9;
    printf("0x%x --> 0x%x", x, setbits(x, p, n, y));    /* 0x95e2 --> 0x95ca */
    return 0;
}

/* return x with the n bits that begin with position p set to the rightmost n
   bits in y */
unsigned setbits(unsigned x, int p, int n, unsigned y)
{
    return  x & ~(~(~0 << n)  << (p+1-n)) |             /* turn bits off */
           (y &   ~(~0 << n)) << (p+1-n);               /* turn bits on */
}
