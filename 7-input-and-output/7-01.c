/* Exercise 7-1. Write a program that converts upper case to lower or lower
   case to upper, depending on the name it is invoked with, as found in
   argv[0]. */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    /* remove unix-style prefix ./ */
    size_t i = strspn(argv[0], "./");
    int (*convert)(int) = isupper((argv[0] + i)[0]) ? toupper : tolower;
    int c;
    while ((c = getchar()) != EOF)
        putchar(convert(c));
    return 0;
}
