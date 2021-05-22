// 21/05/22 = Sat

#include <stdio.h>

void swap(int *a, int *b)
{
  if (a == b)
    return;
  int t;
  t = *a;
  *a = *b;
  *b = t;
}

int icmp(const int *p, const int *q)
{
  return (*p > *q)  - (*p < *q);
}

void qsort2(int * const b, int * const e, int (*cmp)(const int *, const int *))
{
  if (e - b <= 1)
    return;
  int *p = b + (e - b) / 2;
  swap(b, p);
  p = b;
  for (int *q = b + 1; q != e; ++q) {
    if (cmp(q, b) < 0) {
      swap(q, p + 1);
      ++p;
    }
  }
  swap(b, p);
  qsort2(b, p, cmp);
  qsort2(p + 1, e, cmp);
}

void print(int a[], int size)
{
  for (int i = 0; i != size; ++i)
    printf("%d ", a[i]);
  putchar('\n');
}

int main()
{
  int a[] = {8, 5, 4, 3, 2, 6, 1, 9, 0, 7};
  print(a, sizeof a / sizeof a[0]);
  printf("----------------------------------------------------------------\n");
  qsort2(a, a + sizeof a / sizeof a[0], icmp);
  print(a, sizeof a / sizeof a[0]);
  return 0;
}
