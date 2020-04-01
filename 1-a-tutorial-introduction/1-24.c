/* Exercise 1-24. Write a program to check a C program for rudimentary syntax
   errors like unmatched parentheses, brackets and braces. Don't forget about
   quotes, both single and double, escape sequences, and comments. (This
   program is hard if you do it in full generality.) */

#include <stdio.h>

#define CLR ' ' /* clear record for last character, for comments checking */
#define BAD 0
#define GOOD 1
#define NO 0
#define YES 1
#define BAD_HEX -1
#define BAD_HEX_VAL -1
#define MAX_VAL 256

int buffer;
int buffering;

int readch(void);
int getch(void);
int ungetch(int c);
int escape(void);
int hexval(void);
int hex(int c);

/* print out a C program, stop at once at rudimentary syntax errors and print
   the error out, including:
    1. unmatched parentheses, brackets, and braces
    2. unterminated comments
    3. unterminated single and double quotes
    4. zero or multiple characters in single quotes
    5. bad escape sequences in single or double quotes
   where an escape sequence is bad if
    (i) it is of an unknown form, e.g. /c or /8 or /xg
    (ii) the integer value it represents is out of range (256) */
int main(void)
{
    int c, lastc, np, nk, nb;

    np = 0; /* number of parentheses */
    nk = 0; /* number of brackets */
    nb = 0; /* number of braces */
    for (lastc = CLR; (c = readch()) != EOF; lastc = c) {
        /* parentheses, brackets, braces */
        if (c == '(')
            ++np;
        else if (c == '[')
            ++nk;
        else if (c == '{')
            ++nb;
        else if (c == ')') {
            --np;
            if (np < 0) {
                printf("\nError: unmatched right parenthesis.\n");
                return 1;
            }
        }
        else if (c == ']') {
            --nk;
            if (nk < 0) {
                printf("\nError: unmatched right bracket.\n");
                return 1;
            }
        }
        else if (c == '}') {
            --nb;
            if (nb < 0) {
                printf("\nError: unmatched right brace.\n");
                return 1;
            }
        }
        /* comments */
        else if (c == '*' && lastc == '/') {
            for (lastc=CLR; (c=readch())!=EOF && !(c=='/' && lastc=='*');
                 lastc=c)
                ;
            if (c == EOF) {
                printf("\nError: unterminated comment.\n");
                return 1;
            }
            c = CLR;
        }
        /* double quotes */
        else if (c == '"') {
            while ((c=readch())!=EOF && c!='\n' && c!='"') {
                if (c == '\\' && escape() != GOOD) {
                    printf("\nError: bad escape sequence.\n");
                    return 1;
                }
            }
            if (c == EOF || c == '\n') {
                printf("\nError: unterminated double-quoted string.\n");
                return 1;
            }
        }
        /* single quotes */
        else if (c == '\'') {
            if ((c=readch()) == '\\' && escape() != GOOD) {
                printf("Error: bad escape sequence.\n");
                return 1;
            }
            else if (c == '\'') {
                printf("\nError: zero character in single quotes.\n");
                return 1;
            }
            else if (c == EOF || c == '\n') {
                printf("\nError: unterminated single-quoted string.\n");
                return 1;
            }
            else if ((c=readch()) != '\'') {
                printf("\nError: multiple characters in single quotes.\n");
                return 1;
            }
        }
    }
    /* final check for unmatched parentheses, brackets, and braces */
    if (np != 0) {
        printf("\nError: unmatched parentheses.\n");
        return 1;
    }
    if (nk != 0) {
        printf("\nError: unmatched brackets.\n");
        return 1;
    }
    if (nb != 0) {
        printf("\nError: unmatched braces.\n");
        return 1;
    }
    return 0;
}

/* get a character with a buffer, print the character out if not printed yet */
int readch()
{
    extern int buffering;
    int c, was_buffering;

    was_buffering = buffering;
    if ((c = getch()) != EOF) {
        if (was_buffering == NO)
            putchar(c);
        return c;
    }
    else
        return EOF;
}

/* get a character with a one-character buffer */
int getch()
{
    extern int buffer;
    extern int buffering;

    if (buffering == YES) {
        buffering = 0;
        return buffer;
    }
    else
        return getchar();
}

/* put a character back to the buffer */
int ungetch(int c)
{
    extern int buffer;
    extern int buffering;

    if (buffering == YES)
        return BAD;
    else {
        buffering = YES;
        buffer = c;
        return GOOD;
    }
}

/* List of escape sequences:
    \a      alert (bell) character
    \b      backspace
    \f      formfeed
    \n      newline
    \r      carriage return
    \t      horizontal tab
    \v      vertical tab
    \\      backslash
    \'      single quote
    \"      double quote
    \?      question mark
    \ooo    octal number
    \xhh    hexadecimal number
   where ooo represents 1 to 3 octal digits, and hh represents 1 or more
   hexadecimal digits. */
int escape(void)
{
    int c, val, cnt;

    if ((c = readch()) == 'a' || c == 'b' || c == 'f' || c == 'n'|| c == 'r' ||
        c == 't' || c == 'v' || c == '\\' || c == '\'' || c == '"' || c == '?')
        return GOOD;
    /* octal number */
    else if ('0' <= c && c <= '7') {
        val = c - '0';
        for (cnt = 1; cnt < 3 && '0' <= (c=readch()) && c <= '7'; ++cnt)
            val = val * 8 + (c - '0');
        if (c < '0' || c > '7')
            ungetch(c);
        if (val >= MAX_VAL) {
            printf("\nWarning: oct escape sequence (%d) out of range.\n", val);
            return BAD;
        }
        return GOOD;
    }
    /* hexadecimal number */
    else if (c == 'x') {
        if ((val=hexval()) == BAD_HEX_VAL)
            return BAD;
        if (val >= MAX_VAL) {
            printf("\nWarning: hex escape sequence (%d) out of range.\n", val);
            return BAD;
        }
        return GOOD;
    }
    return BAD;
}

int hexval(void)
{
    int c, h, val, state;

    val = 0;
    state = BAD;
    while ((h=hex(c=readch())) != BAD_HEX) {
        val = val * 16 + h;
        state = GOOD;
    }
    ungetch(c);
    if (state == BAD) {
        printf("\nError: \\x used with no following hex digits.\n");
        return BAD_HEX_VAL;
    }
    return val;
}

int hex(int c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    if ('A' <= c && c <= 'F')
        return c - 'A' + 10;
    if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
    return BAD_HEX;
}

/* Note: With a little advanced syntax, this program could be made simpler. */
