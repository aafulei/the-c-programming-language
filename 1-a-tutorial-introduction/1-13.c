/* Exercise 1-13. Write a program to print a histogram of the lengths of words
   in its input. It is easy to draw the histogram with the bars horizontal; a
   vertical orientation is more challenging. */

#include <stdio.h>

#define IN 1        /* inside a word */
#define OUT 0       /* outside a word */
#define MAXWORD 10  /* word length buckets: [1, ..., (MAXWORD and above)] */
#define MAXHIST 20  /* max height of histogram bars */

int main(void)
{
    int c, n, max, height, state, i, j;
    int count[MAXWORD];

    /* initialize array */
    for (i = 0; i < MAXWORD; ++i)
        count[i] = 0;
    /* count word lengths */
    n = 0;
    state = OUT;
    while ((c = getchar()) != EOF) {
        if (state == OUT) {
            if (c != ' ' && c != '\t' && c != '\n') {
                n = 1;
                state = IN;
            }
        }
        else {
            if (c != ' ' && c != '\t' && c != '\n')
                ++n;
            else {
                if (n < MAXWORD)
                    ++count[n-1];
                else    /* word with length >= MAXWORD */
                    ++count[MAXWORD-1];
                n = 0;
                state = OUT;
            }
        }
    }
    /* scale histogram */
    max = 0;
    for (i = 0; i < MAXWORD; ++i)
        if (count[i] > max)
            max = count[i];
    if (max == 0)   /* be careful of empty input (zero division) */
        max = 1;
    if (max > MAXHIST)
        height = MAXHIST;
    else
        height = max;
    /* print histogram */
    printf("Horizontal Histogram\n");
    for (i = 0; i < MAXWORD; ++i) {
        printf("[%2d](%2d) ", i+1, count[i]);
        for (j = 0; j < count[i] * height / max; ++j)
            putchar('*');
        putchar('\n');
    }
    printf("Vertical Histogram\n");
    for (j = height; j >= 1; --j) {
        for (i = 0; i < MAXWORD; ++i)
            if (count[i] * height / max >= j)
                printf("  * ");
            else
                printf("    ");
        putchar('\n');
    }
    for (i = 0; i < MAXWORD; ++i)
        printf("(%2d)", count[i]);
    putchar('\n');
    for (i = 0; i < MAXWORD; ++i)
        printf("[%2d]", i+1);
    putchar('\n');
}

/* Note: Don't forget to initialize array elements to zero! */
