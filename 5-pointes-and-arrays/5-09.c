/* Exercise 5-9. Rewrite the routines day_of_year and month_day with pointers
   instead of indexing. */

static char daytab[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

int day_of_year(int year, int month, int day)
{
    int i, leap, **ptab = daytab;

    leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
    for (i = 0; i < month; i++)
        day += *(*(daytab + leap) + i);
    return day;
}

void month_day(int year, int yearday, int *pyear, int *pmonth, int *pday)
{
    int i, leap, day;

    leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
    *pyear = year;
    *pmonth = leap ? *daytab : *(daytab + 1);
    for (day = 0; day < yearday; ) {
        day += daytab
    }
}

