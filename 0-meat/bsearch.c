#include <stdio.h>
#include <stdlib.h>

int binary_search(char *a, char key, int size)
{
  int lo = 0, hi = size - 1, mi;
  while (lo <= hi) {
    mi = lo + (hi - lo) / 2;
    if (key == a[mi])
      return mi;
    if (key < a[mi])
      hi = mi - 1;
    else
      lo = mi + 1;
  }
  return -1;
}

char *binary_search_2(char *a, char key, int size)
{
  char *b = a, *e = a + size, *m;
  while (b < e) {
    m = b + (e - b) / 2;
    if (key == *m)
      return m;
    if (key < *m)
      e = m;
    else
      b = m + 1;
  }
  return NULL;
}

int main()
{
  char a[] = "bcdkmp";
  char *res, c;
  for (char key = 'a'; key <= 'z'; ++key) {
    res = binary_search_2(a, key, 6);
    if (res)
      c = *res;
    else
      c = '.';
    printf("%c in \"%s\" @ %d (%c)\n", key, a, binary_search(a, key, 6), c);
  }
  return 0;
}
