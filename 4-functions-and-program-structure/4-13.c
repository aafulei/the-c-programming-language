/* Exercise 4-13. Write a recursive version of the function reverse(s), which
   reverses the string s in place. */

#include <stdio.h>

#define MAXLINE 1000

int getline(char [], int);
void reverse(char []);
void rev(char [], int);
void swap(char [], int, int);

/* test: get a line from input and print the reversed string */
int main(void)
{
    char line[MAXLINE];

    while (getline(line, MAXLINE) > 0) {
        reverse(line);
        printf("%s\n", line);
    }
    return 0;
}

/* reverse string */
void reverse(char s[])
{
    rev(s, 0);
}

/* go to string tail, then swap head and tail and move inwards */
void rev(char s[], int i)
{
    if (s[i] == '\0')
        swap(s, 0, i-1);
    else
        rev(s, i+1);
}

/* swap head and tail and move inwards */
void swap(char s[], int i, int j)
{
    char t;

    if (i < j) {
        t = s[i], s[i] = s[j], s[j] = t;
        swap(s, i+1, j-1);
    }
}

/* easy getline: read a line, stop at max len or EOF or newline, return len */
int getline(char s[], int lim)
{
    int c, i;

    for (i = 0; i < lim-1 && (c=getchar()) != EOF && c != '\n'; i++)
        s[i] = c;
    s[i] = '\0';
    return i;
}
