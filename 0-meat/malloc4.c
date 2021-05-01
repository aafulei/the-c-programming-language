// 21/05/01 = Sat

#include <stddef.h>
#include <unistd.h>

#define MIN_REQUEST 100
#define BAD_REQUEST (void *)(-1)

struct header
{
  size_t size;
  struct header* next;
};

static struct header g_base;
static struct header *g_prev = NULL;

struct header *request_from_os(size_t);
void free(void *);

void *malloc(size_t nb)
{
  size_t nu = (nb + sizeof(struct header) - 1) / sizeof(struct header);
  if (!g_prev) {
    g_prev->next = g_prev = &g_base;
    g_prev->size = 0;
  }
  for (struct header *p = g_prev, *q = g_prev->next;
    /* empty */;
    p = q, q = q->next) {
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
  free(res + 1);
  return g_prev;
}

void free(void *ptr)
{
  if (!ptr)
    return;
  struct header *p = g_prev;
  struct header *x = (struct header *)ptr - 1;
  struct header *q = g_prev->next;
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
}

#include <stdio.h>

int main()
{
  char *res = malloc(4 * 1024 - sizeof(struct header));
  if (res) {
    res[4 * 1024 - sizeof(struct header) - 2] = 'x';
    res[4 * 1024 - sizeof(struct header) - 1] = '\0';
    printf("%s\n", &res[4 * 1024 - sizeof(struct header) - 2]);
  }
  else {
    printf("malloc failed!\n");
  }
  free(res);
  return 0;
}
