/* Exercise 8-8. Write a routine bfree(p,n) that will free any arbitrary block
 * p of n characters into the free list maintained by malloc and free. By using
 * bfree, a user can add a static or external array to the free list at any
 * time. */

/*
gcc 8-08.c -Wno-deprecated-declarations
*/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_NUM_BYTES 4096
#define MIN_REQUEST   100
#define BAD_REQUEST   (void *)(-1)

struct header
{
  size_t size;
  struct header *next;
};

static struct header g_base;
static struct header *g_prev = NULL;

struct header *request_from_os(size_t);
int free_(void *);

void *malloc_(size_t nb)
{
  /* ADD */
  if (nb > MAX_NUM_BYTES) {
    fprintf(stderr, "required too much: required %zu, more than max=%zu\n", nb,
            MAX_NUM_BYTES);
    return NULL;
  }
  size_t nu = (nb + sizeof(struct header) - 1) / sizeof(struct header);
  if (!g_prev) {
    g_prev->next = g_prev = &g_base;
    g_prev->size = 0;
  }
  for (struct header *p = g_prev, *q = g_prev->next;
       /* empty */; p = q, q = q->next) {
    if (q->size == nu) {
      p->next = q->next;
      g_prev = p;
      return q + 1;
    }
    if (q->size > nu) {
      size_t rest = q->size - (nu + 1);
      q->size = rest;
      q += rest + 1;
      q->size = nu;
      g_prev = p;
      return q + 1;
    }
    if (q == g_prev) {
      if ((q = request_from_os(nu)) == NULL)
        return NULL;
    }
  }
}

struct header *request_from_os(size_t nu)
{
  if (nu < MIN_REQUEST)
    nu = MIN_REQUEST;
  struct header *res = sbrk(sizeof(struct header) * (nu + 1));
  if (res == BAD_REQUEST)
    return NULL;
  res->size = nu;
  free_(res + 1);
  return g_prev;
}

int free_(void *ptr)
{
  // need to init in case free_() is called before malloc_()!
  if (!g_prev) {
    g_prev->next = g_prev = &g_base;
    g_prev->size = 0;
  }
  if (!ptr)
    return 0;
  struct header *p = g_prev;
  struct header *x = (struct header *)ptr - 1;
  struct header *q = g_prev->next;
  /* ADD */
  if (!x->size || x->size > MAX_NUM_BYTES) {
    fprintf(stderr, "not valid free_()!\n");
    return 1;
  }
  while (!(p < x && x < q || (q <= p) && (x > p || x < q))) {
    p = q;
    q = q->next;
  }
  p->next = x;
  x->next = q;
  if (x + x->size + 1 == q) {
    x->size += q->size + 1;
    x->next = q->next;
  }
  if (p + p->size + 1 == x) {
    p->size += x->size + 1;
    p->next = x->next;
  }
  g_prev = p;
  return 0;
}

// add num bytes pointed by ptr to free list
int bfree_(char *ptr, size_t num)
{
  // need to init in case bfree_() is called before malloc_()!
  if (!g_prev) {
    g_prev->next = g_prev = &g_base;
    g_prev->size = 0;
  }
  if (!ptr)
    return 0;
  if (num <= sizeof(struct header) * 2) {
    size_t z = sizeof(struct header) * 2;
    fprintf(stderr, "too small num of bytes for bfree_(), requries %zu\n", z);
    return 1;
  }
  struct header *p = g_prev;
  struct header *x = (struct header *)ptr;
  struct header *q = g_prev->next;
  // make header
  x->size = num / sizeof(struct header) - 1;
  while (!(p < x && x < q || (q <= p) && (x > p || x < q))) {
    p = q;
    q = q->next;
  }
  p->next = x;
  x->next = q;
  if (x + x->size + 1 == q) {
    x->size += q->size + 1;
    x->next = q->next;
  }
  if (p + p->size + 1 == x) {
    p->size += x->size + 1;
    p->next = x->next;
  }
  g_prev = p;
  return 0;
}

#include <stdio.h>

int main()
{
  free_(NULL);
  printf("debug: g_prev = %p\n", g_prev);
  printf("debug: g_prev->next = %p\n", g_prev->next);
  printf("debug: g_prev->next->next = %p\n", g_prev->next->next);
  char _array_for_malloc[1000];
  bfree_(_array_for_malloc, 1000);
  printf("debug: &_array_for_malloc[0] = %p\n", &_array_for_malloc[0]);
  printf("debug: g_prev = %p\n", g_prev);
  printf("debug: g_prev->next = %p\n", g_prev->next);
  printf("debug: g_prev->next->next = %p\n", g_prev->next->next);
  printf("debug: sizeof _array_for_malloc = %zu\n", sizeof _array_for_malloc);
  printf("debug: g_prev->next->size * sizeof(struct header) = %zu\n",
         g_prev->next->size * sizeof(struct header));
  return 0;
}
