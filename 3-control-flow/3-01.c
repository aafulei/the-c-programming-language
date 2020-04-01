/* Exercise 3-1. Our binary search makes two tests inside the loop, when one
   would suffice (at the price of more tests outside). Write a version with
   only one test inside the loop and measure the difference in run-time. */

/* See Section 3.3 (page 58) for the original binsearch.
    int binsearch(int x, int v[], int n)
    {
        int low, high, mid;
        low = 0;
        high = n - 1;
        while (low <= high) {
            mid = (low + high) / 2;
            if (x < v[mid])
                high = mid + 1;
            else if (x  > v[mid])
                low = mid + 1;
            else
                return mid;
        }
        return -1;
    } */

#include <stdio.h>

int binsearch(int x, int v[], int n);

/* search for -1,...,10 in an array ranging from 0 to 9 */
int main(void)
{
    int x;
    int v[] = {0,1,2,3,4,5,6,7,8,9};

    for (x = -1; x <= 10; x++)
        printf("search for %d in v[]: pos = %d\n", x, binsearch(x, v, 10));
    return 0;
}

/* modified binsearch: one test in loop, more tests outside */
int binsearch(int x, int v[], int n)
{
    int low, high, mid;

    low = 0;
    high = n - 1;
    mid = (low + high) / 2;
    while (low <= high && x != v[mid]) {
        if (x < v[mid])
            high = mid - 1;
        else
            low = mid + 1;
        mid = (low + high) / 2;
    }
    if (x == v[mid])
        return mid;
    else
        return -1;
}
