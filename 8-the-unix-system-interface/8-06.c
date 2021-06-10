/* Exercise 8-6. The standard library function calloc(n,size) returns a pointer
 * to n objects of size size, with the storage initialized to zero. Write
 * calloc, by calling malloc or by modifying it. */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *calloc_(size_t n, size_t size)
{
  void *ret = malloc(n * size);
  memset(ret, 0, n * size);
  return ret;
}

int main()
{
  int *a = calloc_(10, sizeof(int));
  for (int i = 0; i != 10; ++i)
    printf("%d: %d\n", i, a[i]);
  char *s = calloc_(10, sizeof(char));
  s[0] = 'a';
  s[1] = 'b';
  s[2] = 'c';
  printf("%s\n", s);
  return 0;
}
