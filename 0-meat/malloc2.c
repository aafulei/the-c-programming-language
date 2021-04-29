// 21/04/29 = Thu

// gcc malloc2.c

#include <stddef.h>
#include <unistd.h>

#define MIN_REQUEST_UNITS 1024
#define BAD_REQUEST       (void *)(-1)

struct header
{
  struct header *next;
  size_t size; // in units, does not include header itself
};

void *malloc(size_t req_bytes);
void free(void *ptr);
struct header *_req_from_os(size_t req_units);

static size_t HEADER_SIZE = sizeof(struct header);
static struct header g_base;
static struct header *g_prev = NULL;

void *malloc(size_t req_bytes)
{
  size_t req_units = (req_bytes + HEADER_SIZE - 1) / HEADER_SIZE;
  if (g_prev == NULL) {
    g_prev = &g_base;
    g_base.next = g_prev;
    g_base.size = 0;
  }
  // search for first fit, starting with g_prev->next
  struct header *p = g_prev;
  struct header *q = g_prev->next;
  while (1) {
    if (q->size == req_units) {
      p->next = q->next;
      g_prev = p;
      return (void *)(q + 1);
    }
    // tricky: double headers - update old one, create a new one and return it
    if (q->size > req_units) {
      size_t diff = q->size - req_units;
      q->size = diff;
      q += diff;
      q->size = req_units;
      g_prev = p;
      return (void *)(q + 1);
    }
    if (q == g_prev) {
      q = _req_from_os(req_units + 1);
      if (q == NULL)
        return NULL;
    }
    p = q;
    q = q->next;
  }
}

struct header *_req_from_os(size_t req_units)
{
  if (req_units < MIN_REQUEST_UNITS)
    req_units = MIN_REQUEST_UNITS;
  void *res = sbrk(req_units * HEADER_SIZE);
  if (res == BAD_REQUEST)
    return NULL;
  ((struct header *)res)->size = req_units;
  free((void *)((struct header *)res + 1));
  return g_prev;
}

// insert memory to free list
void free(void *ptr)
{
  struct header *x = (struct header *)(ptr)-1;
  struct header *p = g_prev;
  struct header *q = g_prev->next;
  // find where to insert x
  while (1) {
    if (p < x && x < q)
      break;
    p = q;
    q = q->next;
    if (q <= p && (x < q || x > p))
      break;
  }
  p->next = x;
  x->next = q;
  // join adjacent blocks, must first do x,q and then p,x
  if (x + x->size + 1 == q) {
    x->size += q->size + 1;
    x->next = q->next;
  }
  if (p + p->size + 1 == x) {
    p->size += x->size + 1;
    p->next = q;
  }
  g_prev = p;
}

#include <stdio.h>

int main()
{
  char *ret = (char *)malloc(6);
  if (ret) {
    ret[0] = 'a';
    ret[1] = 'b';
    ret[2] = 'c';
    ret[3] = 'd';
    ret[4] = 'e';
    ret[5] = '\0';
    printf("%s\n", ret);
    printf("%zu\n", sizeof ret);
  }
  free(ret);
  return 0;
}
