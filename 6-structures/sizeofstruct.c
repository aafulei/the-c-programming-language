#include <stdio.h>

int main()
{
  struct foo {
      char a[4];
      char *c;
  } bar;
  // char s[] = "hello";
  // bar.c = s;
  printf("%zu", sizeof(bar));
  return 0;
}
