/* Exercise 2-7. Write a function invert(x,p,n) that returns x with the n bits
   that begin at position p inverted (i.e., 1 changed into 0 and vice versa),
   leaving the others unchanged. */

#include <stdio.h>

unsigned invert(unsigned x, int p, int n);

/* Let x=0x95e2, p=5, n=3.
Invert 3 bits of x (100) (to 011).
pos    [FEDC BA98 7654 3210]
[x]
bin     1001 0101 1110 0010
hex        9    5    e    2
mask                ** *
[r]
bin     1001 0000 1101 1010
hex        9    5    d    a
*/
int main(void)
{
    unsigned x, y;
    int p, n;

    x = 0x95e2;
    p = 5;
    n = 3;
    printf("0x%x --> 0x%x", x, invert(x, p, n));    /* 0x95e2 --> 0x95da */
    return 0;
}

/* return x with the n bits that begin with position p inverted */
unsigned invert(unsigned x, int p, int n)
{
    return x ^ ~(~0 << n) << (p+1-n);
}
