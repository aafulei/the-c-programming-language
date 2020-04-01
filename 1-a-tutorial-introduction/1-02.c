/* Exercise 1-2. Experiment to find out what happens when printf's argument
   string contains \c, where c is some character not listed above. */

/* Answer: In general, it is an undefined behavior if the character following \
   is not one of those specified. */

#include <stdio.h>

int main(void)
{
    printf("\c");   /* warning: unknown escape sequence: '\c'; print out 'c' */
    printf("\+");   /* warning: unknown escape sequence: '\+'; print out '+' */
    printf("\0");   /* print out nothing */
    printf("\1");   /* print out a non-graphic symbol <0x01> */
 /* printf("\x"); *//* error: \x used with no following hex digits */
    return 0;
}
