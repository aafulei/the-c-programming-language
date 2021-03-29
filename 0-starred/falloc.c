#include <stddef.h>

typedef double Align;

union header
{
    struct
    {
        union header * ptr;
        unsigned       size;
    } s;
    Align x;
};

typedef union header Header;

static Header   base;
static Header * freep = NULL;

void * malloc(unsigned nbytes)
{
    Header * p, * prevp;
    Header * morecore(unsigned);
    unsigned nunits;
    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1; /* round up */
    if ((prevp = freep) == NULL) {
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }
}

int main() {}
