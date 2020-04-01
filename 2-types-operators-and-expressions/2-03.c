/* Exercise 2-3. Write a function htoi(s), which converts a string of
   hexadecimal digits (including an optional 0x or 0X) into its equivalent
   integer value. The allowable digits are 0 through 9, a through f, and A
   through F. */

#include <stdio.h>

#define MAXLINE 9   /* max 8 hex digits */
#define OUT 0
#define IN 1

int htoi(char s[]);
int getline(char s[], int lim);

/* get a line of hex digits, print the equivalent integer value */
int main(void)
{
    int val;
    char line[MAXLINE];

    while (getline(line, MAXLINE) > 0)
        printf("%d\n", htoi(line));
    return 0;
}

/* convert a string of hex digits to equivalent integer value */
int htoi(char s[])
{
    int i, val, state;

    val = 0;
    i = 0;
    if (s[i] == '0' ) {
        ++i;
        if (s[i] == 'x' || s[i] == 'X')
            ++i;
    }
    for (state = IN; state == IN; ++i) {
        if ('0' <= s[i] && s[i] <= '9')
            val = val * 16 + s[i] - '0';
        else if ('a'<= s[i] && s[i] <= 'f')
            val = val * 16 + s[i] - 'a' + 10;
        else if ('A'<= s[i] && s[i] <= 'F')
            val = val * 16 + s[i] - 'A' + 10;
        else
            state = OUT;
    }
    return val;
}

/* modified getline: read a line into s, return original length */
int getline(char s[], int lim)
{
    int c, i;

    for (i = 0; (c = getchar()) != EOF && c != '\n'; ++i)
        if (i < lim - 1)
            s[i] = c;
    if (c == '\n') {
        if (i < lim - 1)
            s[i] = c;
        ++i;
    }
    if (i < lim)
        s[i] = '\0';
    else
        s[lim-1] = '\0';
    return i;
}

/* Note: This program assumes at least one valid hex digit at the beginning of
   a non-empty line. If int has 32 bits, the program assumes the input is not
   larger than 7FFFF FFFF. */
