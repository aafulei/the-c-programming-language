/* Exercise 1-6. Verify that the expression getchar() != EOF is 0 or 1. */

/* Answer: `getchar() != EOF` is 1 for any keyboard input, except for EOF
   (Ctrl+D in Linux, or Ctrl+Z in Windows), in which case the value is 0. */

#include <stdio.h>

int main(void)
{
    printf("%d", getchar() != EOF);
}

/* Note: If you ever thought about using `putchar(getchar() != EOF)`, you might
   not have a clear understanding about the difference between `putchar(i)` and
   `printf("%d", i)`. `putchar(i)` takes an integer value i and prints out the
   character according to the integer value i in the machine's character set.
   For example, assuming the ASCII character set, `putchar(65)` takes integer
   value 65, and prints out letter A, after looking up 65 in the character set.
   This is fundamentally different from what we want to do here. We don't need
   to consult the character set. No conversion is needed. We simply offer an
   integer value i, and want to print it out. This should be done by
   `printf("%d", i)`. */
