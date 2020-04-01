/* Exercise 4-2. Extend atof to handle scientific notation of the form
   123.45e-6, where a floating-point number may be followed by e or E and an
   optionally signed exponent. */

#include <ctype.h>
#include <stdio.h>

/* test */
int main(void)
{
    double atof(char []);

    printf("%g\n", atof("123.45"));     /* 123.45 */
    printf("%g\n", atof("123.45e0"));   /* 123.45 */
    printf("%g\n", atof("123.45e1"));   /* 1234.5 */
    printf("%g\n", atof("123.45e-1"));  /* 12.345 */
    printf("%g\n", atof("-123.45e1"));  /* -1234.5 */
    printf("%g\n", atof("-1.2345e2"));  /* -123.45 */
    printf("%g\n", atof("123.45e-6"));  /* 0.00012345 */
    printf("%g\n", atof("1.2345E+6"));  /* 1.2345e+006 */
    return 0;
}

/* modified atof: convert characters to floating-point number */
double atof(char s[])
{
    double val, power, expval;
    int i, sign, expsign;

    for (i=0; isspace(s[i]); i++)   /* skip white space */
        ;
    sign = (s[i] == '-' ? -1 : 1);
    if (s[i] == '+' || s[i] == '-')
        i++;
    for (val=0.0; isdigit(s[i]); i++)
        val = val * 10 + (s[i] - '0');
    if (s[i] == '.')
        i++;
    for (power=1.0; isdigit(s[i]); i++) {
        val = val * 10 + (s[i] - '0');
        power *= 10;
    }
    if (tolower(s[i]) == 'e')
        i++;
    expsign = (s[i] == '-' ? -1 : 1);
    if (s[i] == '+' || s[i] == '-')
        i++;
    for (expval=0; isdigit(s[i]); i++)
        expval = expval * 10 + (s[i] - '0');
    while (expval-- > 0)
        power *= (expsign == -1 ? 10 : 0.1);
    return sign * val / power;
}
