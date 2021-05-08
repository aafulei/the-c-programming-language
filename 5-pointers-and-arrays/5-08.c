/* Exercise 5-8. There is no error checking in day_of_year or month_day.
   Remedy this defect.*/

#include <stdio.h>

static char daytab[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

/* day_of_year: set day of year from month & day */
int day_of_year(int year, int month, int day)
{
    int i, leap;

    leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
    /* error checking */
    if (year < 0 || month < 0 || month > 12 || day < 0 || day > daytab[leap][month])
        return -1;
    for (i = 1; i < month; i++)
        day += daytab[leap][i];
    return day;
}

/* month_day: set month, day from day of year */
void month_day(int year, int yearday, int *pmonth, int *pday)
{
    int i, leap;

    leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
    /* error checking */
    if (year < 0 || yearday <= 0 || yearday > 365 + leap) {
        *pmonth = *pday = -1;
        return;
    }
    for (i = 1; yearday > daytab[leap][i]; i++)
        yearday -= daytab[leap][i];
    *pmonth = i;
    *pday = yearday;
}

int main(int argc, char *argv[])
{
    int year, month, day;

    printf("enter year, month, day \n");
    scanf("%d%d%d", &year, &month, &day);
    printf("day of year = %d\n", (day = day_of_year(year, month, day)));
    month_day(year, day, &month, &day);
    printf("validation: year = %d, month = %d, day = %d\n", year, month, day);
    return 0;
}