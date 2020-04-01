/* Exercise 5-3. Write a pointer version of the function strcat that we showed
   in Chapter 2: strcat(s,t) copies the string t to the end of s. */

/* See Section 2.8 (page 48) for the original strcat
    void strcat(char s[], char t[])
    {
        int i, j;
        i = j = 0;
        while (s[i] != '\0')
            i++;
        while ((s[i++] = t[j++]) != '\0')
           ;
    } */

#include <stdio.h>

void strcat(char *, char *);

/* test: concatenate strings and print out Hello, World! */
int main(void)
{
    char s[100] = "Hello, ";
    char t[] = "World!";

    strcat(s, t);
    printf("%s", s);
    return 0;
}

/* strcat: concatenate t to end of s; s must be big enough; pointer version */
void strcat(char *s, char *t)
{
    while (*s != '\0')
        s++;
    while (*s++ = *t++)
        ;
}

/* Note: There might be a compiler warning since the signature for the built-in
   strcat is char *strcat(char *, const char *); */
