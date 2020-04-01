/* Exercise 1.15. Rewrite the temperature conversion program of Section 1.2 to
   use a function for conversion. */

#include <stdio.h>

float celsius(float fahr);

/* print Fahrenheit-Celsius table for fahr = 0, 20, ..., 300;
   floating-point version, using a function */
int main(void)
{
    float fahr;
    float lower, upper, step;

    lower = 0;      /* lower limit of temperatuire scale */
    upper = 300;    /* upper limit */
    step = 20;      /* step size */

    fahr = lower;
    while (fahr <= upper) {
        printf("%3.0f %6.1f\n", fahr, celsius(fahr));   /* modified */
        fahr = fahr + step;
    }
}

float celsius(float fahr)
{
    return (5.0/9.0) * (fahr-32.0);
}
