/* Exercise 7-4. Write a private version of scanf analogous to minprintf from
 * the previous section. */

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>

int isalpha_p(char c)
{
  return isalpha(c) || c == '%';
}

void minscanf(char *fmt, ...)
{
  va_list vl;
  va_start(vl, fmt);
  char *pc;
  int *pi;
  float *pf;
  for (char *p = fmt; *p; ++p) {
    if (*p != '%') {
      if (getchar() != *p);
        printf("error: expect %c\n", *p);
      continue;
    }
    else {
      char spec[100];
      char *q = spec;
      *q++ = '%';
      ++p;
      while (*p && !isalpha_p(*p)) {
        *q++ = *p++;
      }
      if (*p)
        *q++ = *p;
      *q = '\0';
      switch (*p) {
        case 'c':
          pc = va_arg(vl, char *);
          scanf(spec, pc);
          break;
        case 'd':
        case 'i':
          pi = va_arg(vl, int *);
          scanf(spec, pi);
          break;
        case 'f':
          pf = va_arg(vl, float *);
          scanf(spec, pf);
          break;
        default:
          printf("error\n");
          break;
      }
    }
  }
  va_end(vl);
}

int main()
{
  char c;
  int i;
  float f;
  minscanf("%cx%d%f", &c, &i, &f);
  printf("c = %c\n", c);
  printf("i = %d\n", i);
  printf("f = %f\n", f);
  return 0;
}
