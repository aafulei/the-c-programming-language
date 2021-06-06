/* Exercise 7-3. Revise minprintf to handle more of the other facilities of
 * printf. */

#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>

int isalpha_p(char c)
{
  return isalpha(c) || c == '%';
}

/* minprintf: minimal printf with variable argument list */
void minprintf(char *fmt, ...)
{
  va_list ap;
  char *p, *sval;
  int ival;
  double dval;
  unsigned long long ullval;
  va_start(ap, fmt);
  for (p = fmt; *p; p++) {
    if (*p != '%') {
      putchar(*p);
      continue;
    }
    char spec[100];
    char *q = spec;
    *q++ = '%';
    while (*(p+1) && !isalpha_p(*(p+1)))
      *q++ = *++p;
    *q++ = *(p+1);
    *q = '\0';
    // printf("spec = %s, *(p+1) = %c\n", spec, *(p+1));
    switch (*++p) {
      case 'd':
      case 'i':
        ival = va_arg(ap, int);
        printf(spec, ival);
        break;
      case 'o':
      case 'x':
      case 'u':
        ival = va_arg(ap, unsigned);
        printf(spec, ival);
        break;
      case 'f':
      case 'e':
      case 'g':
        dval = va_arg(ap, double);
        printf(spec, dval);
        break;
      case 's':
        sval = va_arg(ap, char *);
        printf(spec, sval);
        break;
      case 'c':
        // warning: 'char' is promoted to 'int' when passed through '...'
        // ival = va_arg(ap, char);
        ival = va_arg(ap, int);
        printf(spec, ival);
        break;
      case 'p':
        ullval = va_arg(ap, unsigned long long);
        printf(spec, ullval);
        break;
      default:
        printf(spec);
        break;
    }
  }
  va_end(ap);
}

int main()
{
  minprintf("%5d\n", -1);
  minprintf("%10.5i\n", -1);
  minprintf("%o\n", -1);
  minprintf("%x\n", -1);
  minprintf("%u\n", -1);
  minprintf("%8.3f\n", 3.1415926);
  minprintf("%4e\n", 3.1415926);
  minprintf("%.3g\n", 3.14);
  minprintf("%20.10s\n", "hello, world!");
  minprintf("%c\n", 'x');
  minprintf("%p\n", &minprintf);
  minprintf("%%\n");
  return 0;
}
