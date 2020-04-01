/* Exercise 2-1. Write a program to determine the ranges of char, short, int,
   and long variables, both signed and unsigned, by printing appropriate values
   from standard headers and by direct computation. Harder if you compute them:
   determine the ranges of the various floating-point types. */

#include <float.h>
#include <limits.h>
#include <stdio.h>

int main(void)
{
    char c, lastc;
    signed char sc, lastsc;
    unsigned char uc, lastuc;
    short h, lasth;
    unsigned short uh, lastuh;
    float f, lastf;
    double b, lastb;
    long double B, lastB;

    printf("---------- from standard headers -----------------------------\n");
    printf("char min = %d\n", CHAR_MIN);
    printf("char max = %d\n", CHAR_MAX);
    printf("signed char min = %d\n", SCHAR_MIN);
    printf("signed char max = %d\n", SCHAR_MAX);
    printf("unsigned signed char min = %d\n", (unsigned char)0);
    printf("unsigned signed char max = %d\n", UCHAR_MAX);
    printf("short min = %hd\n", SHRT_MIN);
    printf("short max = %hd\n", SHRT_MAX);
    printf("unsigned short min = %hu\n", (unsigned short)0);
    printf("unsigned short max = %hu\n", USHRT_MAX);
    printf("int min = %d\n", INT_MIN);
    printf("int max = %d\n", INT_MAX);
    printf("unsigned min = %u\n", 0U);
    printf("unsigned max = %u\n", UINT_MAX);
    printf("long min = %ld\n", LONG_MIN);
    printf("long max = %ld\n", LONG_MAX);
    printf("unsigned long min = %lu\n", 0UL);
    printf("unsigned long max = %lu\n", ULONG_MAX);
    printf("float min = %g\n", FLT_MIN);
    printf("float max = %g\n", FLT_MAX);
    printf("double min = %lg\n", DBL_MIN);
    printf("double max = %lg\n", DBL_MAX);
    printf("long double min = %Lg\n", LDBL_MIN);
    printf("long double max = %Lg\n", LDBL_MAX);
    printf("---------- by direct computation ----------------------------\n");
    /* char */
    lastc = 1;
    for (c = 0; c < lastc; --c)
        lastc = c;
    printf("char min = %d\n", lastc);
    lastc = 0;
    for (c = 1; c > lastc; ++c)
        lastc = c;
    printf("char max = %d\n", lastc);
    /* signed char */
    lastsc = 1;
    for (sc = 0; sc < lastsc; --sc)
        lastsc = sc;
    printf("signed char min = %d\n", lastsc);
    lastsc = 0;
    for (c = 1; sc > lastsc; ++sc)
        lastsc = sc;
    printf("signed char max = %d\n", lastsc);
    /* unsigned char */
    printf("unsigned char min = %d\n", (unsigned char)0);
    for (uc = 1; uc != 0; ++uc)
        lastuc = uc;
    printf("unsigned char max = %d\n", lastuc);
    lasth = 1;
    for (h = 0; h < lasth; --h)
        lasth = h;
    /* short */
    printf("short min = %hd\n", lasth);
    lasth = 0;
    for (h = 1; h > lasth; ++h)
        lasth = h;
    printf("short max = %hd\n", lasth);
    /* unsigned short */
    printf("unsigned short min = %hu\n", (unsigned short)0);
    for (uh = 1; uh != 0; ++uh)
        lastuh = uh;
    printf("unsigned short max = %hu\n", lastuh);
    /* int */
    printf("int min = %d\n", ~(~0U >> 1));
    printf("int max = %d\n", ~0U >> 1);
    /* unsigned int */
    printf("unsigned int min = %u\n", 0U);
    printf("unsigned int max = %u\n", ~0U);
    /* long */
    printf("long min = %ld\n", ~(~0UL >> 1));
    printf("long max = %ld\n", ~0UL >> 1);
    /* unsigned long */
    printf("unsigned long min = %lu\n", 0UL);
    printf("unsigned long max = %lu\n", ~0UL);
    /* float */
    for (f = 0.f; 0.f + f - f == 0.f; f = f + 1e32f)
        lastf = f;
    printf("float max (approx) = %g\n", lastf);
    /* double */
    for (b = 0.0; 0.0 + b - b == 0.0; b = b + 1e302)
        lastb = b;
    printf("double max (approx) = %lg\n", lastb);
    /* long double */
    for (B = 0.0L; 0.0 + B - B == 0.0L; B = B + 1e4926L)
        lastB = B;
    printf("long double max (approx) = %Lg\n", lastB);
    return 0;
}

/* Note: This problem can get really hard if being rigorous. We demonstrate
   multiple approaches here to compute the ranges of the data types, yet the
   only ways that are both invulnerable and feasible are the ones that overflow
   the small unsigned integer types, and the ones that do bitwise operations on
   unsigned integers. Note that
    0. char can be either signed or unsigned;
    1. overflowing a signed integer has undefined behavior;
    2. overflowing a large (e.g. 64-bit) unsigned integer (incrementing by 1)
       is computationally impossible;
    3. theoretically there are 3 ways to represent a negative integer in C:
        (1) two's complement,
        (2) one's complement,
        (3) sign/magnitude;
    4. right shifting a signed integer is implementation-dependent;
    5. computed max values for floating-point types are just approximations;
    6. not sure how to compute the normalized min for floating-point types;
    7. even from the standard header LDBL_MAX does not display correctly on my
       Windows 10 (okay on Ubuntu 18.04 LTS though) --
        [Windows compiler version]
        gcc (x86_64-posix-seh-rev0, Built by MinGW-W64) 8.1.0.
   By the way, by, for example, "double min", we mean the normalized min. */
