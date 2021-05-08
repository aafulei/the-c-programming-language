/* Exercise 5-9. Rewrite the routines day_of_year and month_day with pointers
   instead of indexing. */

#include <stdio.h>

static char daytab[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

int day_of_year(int year, int month, int day)
{
    int i, leap;

    leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
    for (i = 0; i < month; i++)
        day += *(*(daytab + leap) + i);
    return day;
}

void month_day(int year, int yearday, int *pyear, int *pmonth, int *pday)
{
    int i, leap, day;

    *pyear = year;
    leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
    char *p = (leap ? *(daytab + 1) : *daytab) + 1;
    for (*pmonth = 1; yearday > *p; ++p) {
        yearday -= *p;
        ++*pmonth;
    }
    *pday = yearday;
}

int main()
{
    int year, month, day;
    month_day(2020, 100, &year, &month, &day);
    printf("year = %d, month = %d, day = %d\n", year, month, day);
    printf("day of year = %d\n", day_of_year(year, month, day));
    return 0;
}
