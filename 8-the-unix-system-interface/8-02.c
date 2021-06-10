/* Exercise 8-2. Rewrite fopen and _fillbuf with fields instead of explicit bit
 * operations. Compare code size and execution speed. */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PERMS 0666 /* RW for owner, group, others */

#define NULL_    0
#define EOF_     (-1)
#define BUFSIZ_  1024
#define OPEN_MAX 20

#define MAXLINE 1000

enum _flags { _READ = 01, _WRITE = 02, _UNBUF = 04, _EOF = 010, _ERR = 020 };

typedef struct _flag_s
{
  unsigned is_read  : 1;
  unsigned is_write : 1;
  unsigned is_unbuf : 1;
  unsigned is_eof   : 1;
  unsigned is_err   : 1;
} FLAG;

typedef struct _iobuf
{
  int cnt;    // characters left
  char *ptr;  // next character position
  char *base; // location of buffer
  // int flag;   // mode of file access
  FLAG flag;
  int fd; // file descriptor
} FILE_;

#define stdin_  (&_iob[0])
#define stdout_ (&_iob[1])
#define stderr_ (&_iob[2])

// FILE_ _iob[OPEN_MAX] = { { 0, (char *)0, (char *)0, _READ, 0 },
//                          { 0, (char *)0, (char *)0, _WRITE, 1 },
//                          { 0, (char *)0, (char *)0, _WRITE | _UNBUF, 2 } };
FILE_ _iob[OPEN_MAX] = { { 0, (char *)0, (char *)0, { 1, 0, 0, 0, 0 }, 0 },
                         { 0, (char *)0, (char *)0, { 0, 1, 0, 0, 0 }, 1 },
                         { 0, (char *)0, (char *)0, { 0, 1, 1, 0, 0 }, 2 } };

int _fillbuf(FILE_ *);
int _flushbuf(int, FILE_ *);

#define feof(p)   ((p)->flag.is_eof)
#define ferror(p) ((p)->flag.is_err)
#define fileno(p) ((p)->fd)

#define getc_(p)    (--(p)->cnt >= 0 ? (unsigned char)*(p)->ptr++ : _fillbuf(p))
#define putc_(x, p) (--(p)->cnt >= 0 ? *(p)->ptr++ = (x) : _flushbuf((x), p))
#define getchar_()  getc_(stdin_)
#define putchar_(x)  putc_((x), stdout_)

FILE_ *fopen_(char *name, char *mode)
{
  int fd;
  FILE_ *fp;
  if (*mode != 'r' && *mode != 'w' && *mode != 'a')
    return NULL_;
  for (fp = _iob; fp < _iob + OPEN_MAX; fp++) {
    // if ((fp->flag & (_READ | _WRITE)) == 0)
    if (!fp->flag.is_read && !fp->flag.is_write)
      break;
  }
  if (fp >= _iob + OPEN_MAX)
    return NULL_;
  if (*mode == 'w')
    fd = creat(name, PERMS);
  else if (*mode == 'a') {
    if ((fd = open(name, O_WRONLY, 0)) == -1)
      fd = creat(name, PERMS);
    lseek(fd, 0L, 2);
  }
  else
    fd = open(name, O_RDONLY, 0);
  if (fd == -1)
    return NULL_;
  fp->fd = fd;
  fp->cnt = 0;
  fp->base = NULL_;
  // fp->flag = (*mode == 'r') ? _READ : _WRITE;
  if (*mode == 'r')
    fp->flag.is_read = 1;
  else
    fp->flag.is_write = 1;
  return fp;
}

int _fillbuf(FILE_ *fp)
{
  int bufsize;
  // if ((fp->flag & (_READ | _EOF | _ERR)) != _READ)
  if (!(fp->flag.is_read && !fp->flag.is_eof && !fp->flag.is_err))
    return EOF_;
  // bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ_;
  bufsize = fp->flag.is_unbuf ? 1 : BUFSIZ_;
  if (fp->base == NULL_)
    if ((fp->base = (char *)malloc(bufsize)) == NULL_)
      return EOF_;
  fp->ptr = fp->base;
  fp->cnt = read(fp->fd, fp->ptr, bufsize);
  if (--fp->cnt < 0) {
    if (fp->cnt == -1) {
      // fp->flag |= _EOF;
      fp->flag.is_eof = 1;
    }
    else {
      // fp->flag |= _ERR;
      fp->flag.is_err = 1;
    }
    fp->cnt = 0;
    return EOF_;
  }
  return (unsigned char)*fp->ptr++;
}

int fgetline(FILE_ *fp, char *line, int lim)
{
  int c, n = 0;
  while ((c = getc_(fp)) != EOF_ && c != '\n') {
    if (n++ < lim) {
      *line++ = c;
    }
  }
  *line = '\0';
  return c;
}

int main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(stderr, "usage: %s file\n", argv[0]);
    return 1;
  }
  FILE_ *fp;
  if ((fp = fopen_(argv[1], "r")) == NULL_) {
    fprintf(stderr, "cannot open %s\n", argv[1]);
    return 1;
  }
  char line[MAXLINE];
  while (fgetline(fp, line, MAXLINE) != EOF_) {
    printf("%s\n", line);
  }

  return 0;
}
