/* Exercise 5-7. Rewrite readlines to store lines in an array supplied by main,
   rather than calling alloc to maintain storage. How much faster is the
   program? */

#include <stdio.h>

#define MAXLINES 5000
#define MAXLEN   100

void m_strcpy(char *dest, char *src)
{
    while (*dest++ = *src++)
        ;
}

int m_getline(char *s, int lim)
{
    int i = 0;
    char c;

    while (i < lim-1 && (c=getchar()) != EOF && c != '\n')
        s[i++] = c;
    s[i] = '\0';
    return i;
}

int readlines(char (*lines)[MAXLEN], int maxlines)
{
    int len, num;
    char *p, a[MAXLEN];

    num = 0;
    while ((len = m_getline(a, MAXLEN)) > 0)
        if (num >= maxlines)
            return -1;
        else
            m_strcpy(lines[num++], a);
   return num;
}

void print(char (*lines)[MAXLEN], int num)
{
    while (num --> 0)
        printf("%s\n", *lines++);
}

int main(void)
{
    char lines[MAXLINES][MAXLEN];
    int num = readlines(lines, 50);
    print(lines, num);
    return 0;
}
