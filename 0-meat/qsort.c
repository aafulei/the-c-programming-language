#include <stdio.h>

void swap(int *a, int *b)
{
  if (a == b)
    return;
  int t;
  t = *a, *a = *b, *b = t;
}

void qsort(int *left, int *right)
{
  void swap(int *, int *);
  if (left >= right)
    return;
  int *pivot = left + (right - left) / 2;
  swap(left, pivot);
  pivot = left;
  for (int *p = left + 1; p <= right; ++p) {
    if (*p < *left) {
      swap(++pivot, p);
    }
  }
  swap(left, pivot);
  qsort(left, pivot - 1);
  qsort(pivot + 1, right);
}

void print(int *a, int n)
{
  while (n-- > 0)
    printf("%d ", *a++);
  putchar('\n');
}

int main()
{
  int array[100];
  printf("enter numbers: ");
  int n = 0;
  for (; n < 100; ++n)
    if (scanf("%d", array + n) != 1)
      break;
  print(array, n);
  qsort(array, array + n - 1);
  print(array, n);
  return 0;
}
