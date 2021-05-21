/* Exercise 7-9. Functions like isupper can be implemented to save space or to
save time. Explore both possibilities. */

#include <stdio.h>

#define isupper_save_time(c) ('A' <= (c) && (c) <= 'Z' ? 1 : 0)

int isupper_save_space(int c)
{
  if ('A' <= c && c <= 'Z')
    return 1;
  else
    return 0;
}

int main()
{
  int c;
  while ((c = getchar()) != EOF) {
    if (isupper_save_space(c))
      putchar(c);
    if (isupper_save_time(c))
      putchar(c);
  }
  return 0;
}
