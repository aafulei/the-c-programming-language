// gcc malloc3.c -Wno-deprecated-declarations

#include <unistd.h>

#define MIN_REQUEST 1000
#define BAD_REQUEST (void *)(-1)

struct header
{
  struct header *next;
  size_t size;
};

struct header *request_from_os(size_t);
void free(void *);

static struct header g_base;
static struct header *g_prev = NULL;

void *malloc(size_t nb)
{
  if (!g_prev) {
    g_prev->next = g_prev = &g_base;
    g_prev->size = 0;
  }
  size_t nu = (nb + sizeof(struct header) - 1) / sizeof(struct header);
  for (struct header *p = g_prev, *q = g_prev->next; /* empty */;
       p = q, q = q->next) {
    if (q->size == nu || q->size == nu + 1) {
      p->next = q->next;
      g_prev = p;
      return q + 1;
    }
    if (q->size > nu + 1) {
      size_t diff = q->size - nu - 1;
      q->size = diff;
      q += diff + 1;
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
  struct header *q = g_prev->next;
  struct header *x = (struct header *)(ptr) - 1;
  while (!(p < x && x < q || q <= p && (p < x || x < q))) {
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
  int num = 4 * 1024 * 1024 - 16;
  char *res = malloc(num);
  if (res) {
    for (size_t i = 0; i != num - 1; ++i)
      res[i] = i % 26 + 'a';
    res[num-1] = '\0';
    printf("%s\n", res + num-100);
  }
  else {
    printf("malloc failed!\n");
  }
  free(res);
  return 0;
}
