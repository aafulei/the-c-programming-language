/* Exercise 1-1. Run the "hello, world" program on your system. Experiment with
   leaving out parts of the program, to see what error messages you get. */

/* Answer: When compiled with `gcc -ansi -pedantic`,
   (1) if `#include <stdio.h>` was missing, compiler would issue a warning:
       warning: incompatible implicit declaration of built-in function 'printf'
   (2) if `\n` was missing, no newline would be printed out following "hello,
       world" (in Linux); however, there is no difference on a Windows console
   (3) if `"` was missing after `\n`, compiler would issue a series of
       errors, with the first one being:
       error: missing terminating " character
   (4) if `;` was missing after `printf`, compiler would issue an error:
       error: expected ';' before 'return'
   (5) if `}` was missing, compiler would issue an error:
       error: expected declaration or statement at end of input */

#include <stdio.h>

int main(void)
{
    printf("hello, world\n");
    return 0;
}
