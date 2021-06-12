/* Exercise 8-7. malloc_ accepts a size request without checking its
 * plausibility; free_ believes that the block it is asked to free_ contains a
 * valid size field. Improve these routines so they make more pains with error
 * checking. */

// gcc 8-07.c -Wno-deprecated-declarations

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

#include <stdio.h>

int main()
{
  char *mem = malloc_(4 * 1024 + 1);
  if (mem) {
    printf("malloc_ success\n");
    free_(mem);
  }
  else
    printf("malloc_ failure\n");

  mem = malloc_(4 * 1024);
  if (mem)
    printf("malloc_ success\n");
  else
    printf("malloc_ failure\n");
  printf("mem size = %zu\n", ((struct header *)mem - 1)->size);
  ((struct header *)mem - 1)->size = 0;
  printf("mem size = %zu\n", ((struct header *)mem - 1)->size);
  free_(mem);
  return 0;
}
