// 21/04/10 = Sat

#include <stdio.h>

#define BUFFER_SIZE 100

char   g_buffer[BUFFER_SIZE];
char * g_free = g_buffer;       // always points to the slot to put back a char

// return char on success, EOF on failure
int getch()
{
    if (g_free == g_buffer)
        return getchar();
    else
        return *--g_free;
}

// return c on success, EOF on failure
int ungetch(char c)
{
    if (g_free < g_buffer + BUFFER_SIZE)
        return *g_free++ = c;
    else
        return EOF;
}
