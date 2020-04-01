/* Exercise 1-23. Write a program to remove all comments from a C program.
   Don't forget to handle quoted strings and character constants properly. C
   comments don't nest. */

#include <stdio.h>

#define CLR ' '

int main(void)
{
    int c, lastc;

    for (lastc=CLR; (c=getchar()) != EOF; lastc=c)
        if (c == '*' && lastc == '/') {
            for (lastc=CLR; (c=getchar()) != EOF && !(c=='/' && lastc=='*');
                 lastc=c)
                ;
            if (c == EOF)   /* allow for bad programs: comments do not close */
                return 1;
            c = CLR;
        }
        else {
            if (lastc == '/')
                putchar(lastc);
            if (c != '/')
                putchar(c);
            if (c == '"' && lastc != '\'' && lastc != '\\') {
                for (lastc=c; (c=getchar()) != EOF && c != '\n' &&
                     !(c == '"' && lastc != '\\');
                     lastc = c)
                    putchar(c);
                if (c == '"')
                    putchar(c);
                else        /* allow for bad programs: quotes do not close */
                    return 1;
            }
        }
    return 0;
}

/* Note: Setting lastc to CLR clears record for the last character. This deals
   with non-examples like (backslash)(asterisk)(backslash), which opens but not
   yet closes a comment. CLR can be any character other than / and *. */
