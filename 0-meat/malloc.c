// gcc -Wno-builtin-declaration-mismatch -Wno-deprecated-declarations malloc.c

#include <unistd.h>

#define MIN_REQUEST_UNITS 1024
#define BAD_REQUEST (void *)(-1)

typedef long align_t;

union header
{
    struct
    {
        union header * next;
        unsigned       size;    // in blocks, does not include header itself
    } s;
    align_t _alignment;
};

typedef union header Header;

void * malloc(unsigned req_bytes);
void free(void * ptr);
Header * _req_from_os(unsigned req_units);

static Header   g_base;
static Header * g_prev = NULL;

void * malloc(unsigned req_bytes)
{
    // round up to a multiple of header sizes
    unsigned req_units = (req_bytes + sizeof(Header) - 1) / sizeof(Header);
    // initialize free list
    if (g_prev == NULL) {
        g_prev = &g_base;
        g_base.s.next = g_prev;
        g_base.s.size = 0;
    }
    // search for first fit, starting with g_prev->s.next
    Header * p = g_prev;
    Header * q = g_prev->s.next;
    while (1) {
        if (q->s.size == req_units) {
            p->s.next = q->s.next;
            g_prev = p;
            return (void *)(q + 1);
        }
        if (q->s.size > req_units) {
            unsigned diff = q->s.size - req_units;
            q->s.size = diff;
            q += diff;
            q->s.size = req_units;
            g_prev = p;
            return (void *)(q + 1);
        }
        if (q == g_prev) {
            q = _req_from_os(req_units + 1);
            if (q == NULL)
                return NULL;
        }
        p = q;
        q = q->s.next;
    }
}

Header * _req_from_os(unsigned req_units)
{
    if (req_units < MIN_REQUEST_UNITS)
        req_units = MIN_REQUEST_UNITS;
    void * ret = sbrk(req_units * sizeof(Header));
    if (ret == BAD_REQUEST)
        return NULL;
    ((Header *)ret)->s.size = req_units;
    free((void *)((Header *)ret + 1));
    return g_prev;
}

// insert memory to free list
void free(void * ptr)
{
    Header * x = (Header *)(ptr) - 1;
    Header * p = g_prev;
    Header * q = g_prev->s.next;
    // find where to insert x
    while (1) {
        if (p < x && x < q)
            break;
        p = q;
        q = q->s.next;
        if (q <= p && (x < q || x > p))
            break;
    }
    p->s.next = x;
    x->s.next = q;
    // join if applicable (note: order matters)
    if (x + x->s.size + 1 == q) {
        x->s.size += q->s.size + 1;
        x->s.next = q->s.next;
    }
    if (p + p->s.size + 1 == x) {
        p->s.size += x->s.size + 1;
        p->s.next = q;
    }
    g_prev = p;
}

#include <stdio.h>

int main()
{
    char * ret = (char *)malloc(6);
    if (ret) {
        ret[0] = 'a';
        ret[1] = 'b';
        ret[2] = 'c';
        ret[3] = 'd';
        ret[4] = 'e';
        ret[5] = '\0';
        printf("%s\n", ret);
    }
    free(ret);
    return 0;
}
