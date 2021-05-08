/* Exercise 5-5. Write versions of the library functions strncpy, strncat, and
   strncmp, which operate on at most the first n characters of their argument
   strings. For example, strncpy(s,t,n) copies at most n characters of t to s.
   Full descriptions are in Appendix B. */

#include <ctype.h>
#include <stdio.h>

char *mystrncpy(char *, char *, int);
char *mystrncat(char *, char *, int);
int mystrncmp(char *, char *, int);

/* test */
int main(void)
{
    char s[100];
    char t[] = "Hello, World!";
    char u[] = " ";
    char v[] = "How do you do? ~!@#$%^&*()";
    int i;

    /* "Hello, World!" */
    printf("%s\n", mystrncpy(s, t, 15));
    /* "Hello, World![0][0]" */
    for (i = 0; i < 15; i++)
        printf((isprint(s[i]) ? "%c" : "[%d]"), s[i]);
    putchar('\n');
    /* "Hello, World! How do you do?" */
    printf("%s\n", mystrncat(mystrncat(s, u, 15), v, 15));
    /* s now points to "Hello, World! How do you do?" */
    printf("%d\n", mystrncmp(s, "Hello", 1));     /* 0 */
    printf("%d\n", mystrncmp(s, "Hello", 5));     /* 0 */
    printf("%d\n", mystrncmp(s, "Hello", 10));    /* >0 (s has "Hello") */
    printf("%d\n", mystrncmp(s, "Hello", 1000));  /* >0 (s has "Hello") */
    printf("%d\n", mystrncmp("Hi", s, 1));        /* 0 */
    printf("%d\n", mystrncmp("Hi", s, 2));        /* >0 (4) */
    printf("%d\n", mystrncmp("Hi", s, 5));        /* >0 (4) */
    printf("%d\n", mystrncmp("Hi", s, 100));      /* >0 (4) */
    return 0;
}

/* Copy at most n characters of string ct to s; return s. Pad with zeros if ct
   has fewer than n characters. No null-character is implicitly appended at the
   end of s if ct has n or more than n characters. */
char *mystrncpy (char *s, char *ct, int n)
{
    char *b = s;

    while (n && (*s++ = *ct++))
        n--;
    while (n--)
        *s++ = '\0';
    return b;
}

/* Concatenate at most n characters of string ct to string s, terminate s
   with '\0'; return s. If ct has fewer than n characters, only the content up
   to the terminating null-character is copied. */
char *mystrncat(char *s, char *ct, int n)
{
    char *b = s;

    while (*s)
        ++s;
    while (n-- && (*s++ = *ct++))
        ;
    *s = '\0';
    return b;
}

/* Compare at most n characters of string cs to string ct; return <0 if cs<ct,
   0 if cs==ct, or >0 if cs>ct. */
int mystrncmp(char *cs, char *ct, int n)
{
    for(; n--; cs++, ct++)
        switch ((*cs != '\0') + 2 * (*ct != '\0')) {
            case 0: return 0;   /* both s and t end */
            case 1: return 1;   /* s contains t */
            case 2: return -1;  /* t contains s */
            case 3: if (*cs != *ct)
                        return *cs - *ct;
        }
    return 0;
}

/* 1. Functions names are prefixed with "my", as printf() may call the library
   functions with the same names, such as strncmp().
   2. Several wrong ways to implement strncpy():
    (1)
    while (n && *s++ = *ct++)
        n--;
    Comment: the pair of parentheses cannot be omitted. = has lower precedence.
    (2)
    while ((*s++ = *ct++) && n)
        n--;
    Comment: Short-circuit evaluation might mess up the counting.
    (3)
    while (n-- && (*s++ = *ct++))
        ;
    Comment: A catastrophe when followed by while (n--) *s++ = '\0'; as n could
    be negative when entering the second while loop. */
