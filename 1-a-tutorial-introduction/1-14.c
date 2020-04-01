/* Exercise 1-14. Write a program to print a histogram of the frequencies of
   different characters in its input. */

#include <stdio.h>

#define MAXHIST 100 /* max height of histogram bars */
#define LOW 33      /* low end of ASCII graphic characters */
#define HIGH 127    /* high end of ASCII graphic characters */

int main(void)
{
    int c, max, height, i, j;
    int count[HIGH-LOW+1];

    /* initialize array */
    for (i = 0; i < HIGH-LOW+1; ++i)
        count[i] = 0;
    while ((c = getchar()) != EOF)
        ++count[c-LOW];
    /* scale histogram */
    max = 0;
    for (i = 0; i < HIGH-LOW+1; ++i)
        if (count[i] > max)
            max = count[i];
    if (max == 0)
        max = 1;
    if (max > MAXHIST)
        height = MAXHIST;
    else
        height = max;
    /* print histogram */
    for (i = 0; i < HIGH-LOW+1; ++i) {
        if (count[i]) {
            printf("[%c](%3d) ", i+LOW, count[i]);
            for (j = 0; j < count[i] * height / max; ++j)
                putchar('*');
            putchar('\n');
        }
    }
}

/* Note: This program makes use of the fact that the graphic characters in the
   ASCII character set range from 33 to 127. Only graphic characters that have
   at least one count will appear in the histogram. */
