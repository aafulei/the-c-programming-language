#include <stdio.h>
#include <unistd.h>

#define BAD_REQUEST (void *)(-1)

int main()
{
  int n;
  printf("how many megabytes do you want? ");
  scanf("%d", &n);
  void *res = sbrk(n * 1000 * 1000);
  printf("%p\n", res);
  if (res != BAD_REQUEST) {
    printf("success!\n");
  }
  else {
    printf("failure!\n");
  }
  return 0;
}
