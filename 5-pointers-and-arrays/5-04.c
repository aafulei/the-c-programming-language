/* Exercise 5-4. Write the function strend(s,t), which returns 1 if the string
   t occurs at the end of the string s, and zero otherwise. */

#include <stdio.h>

int strend(char *, char *);

/* test */
int main(void)
{
    printf("%d\n", strend("", ""));                             /* 1 */
    printf("%d\n", strend("happy", ""));                        /* 0 */
    printf("%d\n", strend("happy", "y"));                       /* 1 */
    printf("%d\n", strend("happy", "newyear"));                 /* 0 */
    printf("%d\n", strend("happy", "happynewyear"));            /* 0 */
    printf("%d\n", strend("happynewyear", "happy"));            /* 0 */
    printf("%d\n", strend("happynewyear", "year"));             /* 1 */
    printf("%d\n", strend("happynewyear", "newyear"));          /* 1 */
    printf("%d\n", strend("happynewyear", "newyear!"));         /* 0 */
    printf("%d\n", strend("happynewyear", "happynewyear"));     /* 1 */
    return 0;
}

/* return 1 if t occurs at the end of s, and 0 otherwise */
int strend(char *s, char *t)
{
    char *b = t;

    while (*s != '\0' && *t != '\0')
        if (*s++ != *t++)
            t = b;
    return *s == '\0' && *t == '\0';
}
