/* Exercise 1-21. Write a program entab that replaces strings of blanks by the
   minimum number of tabs and blanks to achieve the same spacing. Use the same
   tab stops as for detab. When either a tab or a single blank would suffice to
   reach a tab stop, which should be given preference? */

/* Answer: When either a tab or a single blank would suffice to reach a tab
   stop, tab will be given preference, as the approach is greedy: so long as
   it is possible to use a tab, use a tab. */

#include <stdio.h>

#define TABSIZE 8

void flush(int nc, int nb);

/* entab: replace blanks in input with minimum number of tabs and blanks */
int main(void)
{
    int c, i, j;

    i = 0;  /* number of characters in current line */
    j = 0;  /* number of blanks in buffer */
    while ((c = getchar()) != EOF) {
        if (c == ' ')
            ++j;
        else if (c == '\t')
            j = j + (TABSIZE - (i + j) % TABSIZE);
        else {
            flush(i, j);
            i = i + j;
            j = 0;
            putchar(c);
            if (c == '\n')
                i = 0;
            else
                ++i;
        }
    }
    flush(i, j);
    return 0;
}

/* given i characters in current line, flush j blanks out of buffer */
void flush(int i, int j)
{
    int k;

    k = TABSIZE - i % TABSIZE;
    while (j >= k) {
        putchar('\t');
        j = j - k;
        k = TABSIZE;
    }
    while (j > 0) {
        putchar(' ');
        --j;
    }
}

/* Note: Assignment operator (+=/-=) and pointers have not been introduced yet;
   otherwise they can be used here. */
