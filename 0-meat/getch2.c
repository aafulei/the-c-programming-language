// 21/04/29 = Thu

#include <stdio.h>

#define BUFSIZE 100

static char g_buffer[BUFSIZE];
static char *g_next = g_buffer;

int getch()
{
  if (g_next == g_buffer)
    return getchar();
  else
    return *--g_next;
}

int ungetch(int c)
{
  if (g_next < g_buffer + BUFSIZE)
    return *g_next++ = c;
  else
    return EOF;
}
