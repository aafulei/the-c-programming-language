/* Exercise 2-9. In a two's complement number system, x &= (x-1) deletes the
   rightmost 1-bit in x. Explain why. Use this observation to write a faster
   version of bitcount. */

/* See Section 2.10 (page 50) for the original bitcount.
    int bitcount(unsigned x)
    {
        int b;
        for (b = 0; x != 0; x >>= 1)
            if (x & 01)
                b++;
        return b;
    } */

/* Answer: Let y=(x-1). Then y+1=x. Visualize the process of adding 1 to a
   binary y. This process makes the rightmost 0-bit in y a 1-bit, while making
   all the 1-bits to the left 0-bits. In this regard,  setting x = x & y
   deletes the rightmost 1-bit in x, while keeping all the 0-bits to the left
   unchanged. */

#include <stdio.h>

int bitcount(unsigned x);

/* Count 1 bits in 0, 0x95e2, 0xffff
bin     1001 0101 1110 0010
hex        9    5    e    2
cnt     +  +  + + +++    +
bin     1111 1111 1111 1111
hex        f    f    f    f
cnt     ++++ ++++ ++++ ++++ */
int main(void)
{
    printf("%d\n", bitcount(0));        /* 0 */
    printf("%d\n", bitcount(0x95e2));   /* 8 */
    printf("%d\n", bitcount(0xffff));   /* 16 */
    return 0;
}

/* modified, faster bitcount: count 1 bits in x */
int bitcount(unsigned x)
{
    int b;

    for (b = 0; x != 0; x &= x-1)
        b++;
    return b;
}
