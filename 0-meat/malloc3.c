#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct header
{
    struct header *next;
    size_t size;
};

struct header g_base;
struct header *g_free = NULL;
struct header *g_prev_free = NULL;

void *malloc(size_t nb)
{
    if (!g_free) {
        g_free->next = g_free = g_prev_free = &g_base;
        g_free->size = 0;
    }
    size_t sz = (nb + sizeof(struct header) - 1) / sizeof(struct header);
    while (1) {
        if (g_free->size == sz) {
            g_prev_free->next = g_free->next;
            struct header *ret = g_free;
            g_free = g_free->next;
            return ret + 1;
        }
        else if (g_free->size > sz) {
            size_t rest = g_free->size - sz;
            g_free->size = rest;
            (g_free + rest) ;

        }
    }
}

