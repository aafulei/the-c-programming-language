/* Exercise 1-22. Write a program to "fold" long input lines into two or more
   shorter lines after the last non-blank character that occurs before the n-th
   column of input. Make sure your program does something intelligent with very
   long lines, and if there are no blanks or tabs before the specified
   column. */

#include <stdio.h>

#define MAXLINE 79  /* max characters in one line */
#define MAXWORD 256 /* word buffer size */
#define TABSIZE 8   /* tab size */
#define NO 0
#define YES 1

int main(void)
{
    int c, i, j, k, breakable;
    char word[MAXWORD]; /* word buffer */

    i = 0;  /* number of characters in current line */
    j = 0;  /* length of blank characters: 1 for blank, to next stop for tab */
    k = 0;  /* length of word in buffer */
    breakable = NO;
    while ((c = getchar()) != EOF)
        if (c == ' ' || c == '\t' || c == '\n') {
            if (k != 0) {
                if (breakable && i + k > MAXLINE) {
                    putchar('\n');
                    i = 0;
                    breakable = NO;
                }
                word[k] = '\0';
                printf("%s", word);
                i = i + k;
                k = 0;
            }
            if (c == '\t')
                j = TABSIZE - j % TABSIZE;
            else
                j = 1;
            if (c == '\n' || i + j > MAXLINE) {
                putchar('\n');
                i = 0;
                breakable = NO;
            }
            else {
                putchar(c);
                i = i + j;
                breakable = YES;
            }
        }
        else
            if (k < MAXWORD-1) {
                word[k] = c;
                ++k;
            }
            else {
                word[k] = '\0';
                printf("%s", word);
                i = i + k;
                k = 0;
            }
    /* Don't forget to flush the buffer! */
    word[k] = '\0';
    printf("%s", word);
    return 0;
}

/* Note: When character is non-blank, buffer; otherwise, flush. Pseudo code:

    Read one character c at a time until EOF:
        if c is non-blank character:
            buffer c to a word
        else:
            if there is a word being buffered:
                if there is room for word:
                    flush word
                else:
                    break a line
                    flush word
            if there is room for blank:
                put down blank
            else:
                break a line
                put down blank
    flush word

   Besides, it is possible to use external variables/functions in this case. */
