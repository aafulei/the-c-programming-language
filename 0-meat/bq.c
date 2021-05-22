#include <stdio.h>

#define numof(a) (sizeof(a) / sizeof(a[0]))

void swap(int *a, int *b)
{
  if (a == b)
    return;
  int t;
  t = *a;
  *a = *b;
  *b = t;
}

void qs(int *const left, int *const right)
{
  if (right <= left)
    return;
  int *p = left + (right - left) / 2;
  swap(p, left);
  p = left;
  for (int *q = left + 1; q <= right; ++q) {
    if (*q < *left) {
      swap(q, p + 1);
      ++p;
    }
  }
  swap(p, left);
  qs(left, p - 1);
  qs(p + 1, right);
}

void qs2(int *const b, int *const e)
{
  if (e - b <= 1)
    return;
  int *p = b + (e - b) / 2;
  swap(b, p);
  p = b;
  for (int *q = b + 1; q != e; ++q) {
    if (*q < *b) {
      swap(q, p + 1);
      ++p;
    }
  }
  swap(b, p);
  qs2(b, p);
  qs2(p + 1, e);
}

int bs(int const *const a, int n, int key)
{
  int lo = 0, hi = n - 1, mi;
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

int const *bs2(int const *b, int const *e, int key)
{
  int const *m;
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

void print(int *a, int n)
{
  while (n-- > 0)
    printf("%d ", *a++);
  putchar('\n');
}

int main()
{
  int a1[] = { 8, 3, 7, 5, 2, 4, 1 };
  print(a1, numof(a1));
  qs(a1, a1 + numof(a1) - 1);
  print(a1, numof(a1));

  int a2[] = { 8, 3, 7, 5, 2, 4, 1 };
  print(a2, numof(a2));
  qs2(a2, a2 + numof(a2));
  print(a2, numof(a2));

  for (int i = 0; i != 10; ++i) {
    int ind = bs(a1, numof(a1), i);
    int const *pos = bs2(a2, a2 + numof(a2), i);
    if (ind != -1)
      printf("%d is found at pos %d", i, ind);
    else
      printf("%d is not found     ", i);
    if (pos)
      printf(" (value check = %d) ", *pos);
    else
      printf(" (no value)");
    putchar('\n');
  }
  return 0;
}
