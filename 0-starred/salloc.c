/* salloc: stack-based allocator, or a simple allocator */

#include <stdio.h>
#include <stddef.h>

#define ALLOCSIZE 10000

static char  allocbuf[ALLOCSIZE];
static char *allocp = allocbuf;

char *alloc(unsigned n)
{
    char *old = NULL;
    if (allocp + n <= allocbuf + ALLOCSIZE) {
        old = allocp;
        allocp += n;
    }
    return old;
}

int afree(char *p)
{
    int ok = 0;
    if (allocbuf <= p && p < allocbuf + ALLOCSIZE) {
        allocp = p;
        ok = 1;
    }
    return ok;
}

int main(void)
{
    return 0;
}
