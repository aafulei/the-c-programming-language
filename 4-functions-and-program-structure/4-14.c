/* Exercise 4-14. Define a macro swap(t,x,y) that interchanges two arguments of
   type t. (Block structure will help.) */

#include <stdio.h>

#define swap(t, x, y) do { t temp; temp = x, x = y, y = temp; } while (0)

/* swap "ab" to "ba" */
int main(void)
{
    char a[] = "ab";

    printf("%s\n", a);  /* ab */
    if (1)
        swap(char, a[0], a[1]);
    else
        ;
    printf("%s\n", a);  /* ba */
    return 0;
}
