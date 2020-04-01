/* Exercise 1-7. Write a program to print the value of EOF. */

#include <stdio.h>

int main(void)
{
    printf("%d", EOF);  /* -1 */
}

/* Note: If you ever thought about using `putchar(EOF)`, you might not have a
   clear understanding about the difference between `putchar(i)` and
   `printf("%d", i)`. See the note on Exercise 1-6 for discussion. */
