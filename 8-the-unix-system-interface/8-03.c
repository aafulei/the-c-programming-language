// need better error handling

/* Exercise 8-3. Design and write _flushbuf, fflush, and fclose. */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSIZ_  10
#define EOF_     (-1)
#define MAXLINE  1000
#define NULL_    0
#define OPEN_MAX 20
#define PERMS    0666 /* RW for owner, group, others */
enum _flags { _READ = 01, _WRITE = 02, _UNBUF = 04, _EOF = 010, _ERR = 020 };
typedef struct _iobuf
{
  int cnt;    // characters left
  char *ptr;  // next character position
  char *base; // location of buffer
  int flag;   // mode of file access
  int fd;     // file descriptor
} FILE_;
FILE_ _iob[OPEN_MAX] = { { 0, NULL_, NULL_, _READ, 0 },
                         { 0, NULL_, NULL_, _WRITE, 1 },
                         { 0, NULL_, NULL_, _WRITE | _UNBUF, 2 } };
#define stdin_  (&_iob[0])
#define stdout_ (&_iob[1])
#define stderr_ (&_iob[2])


int _fillbuf(FILE_ *);
int _flushbuf(int, FILE_ *);
int fflush_(FILE_ *fp);



#define feof(p)   ((p)->flag.is_eof)
#define ferror(p) ((p)->flag.is_err)
#define fileno(p) ((p)->fd)

#define getc_(p)    (--(p)->cnt >= 0 ? (unsigned char)*(p)->ptr++ : _fillbuf(p))
#define putc_(x, p) (--(p)->cnt >= 0 ? *(p)->ptr++ = (x) : _flushbuf((x), p))
#define getchar_()  getc_(stdin_)
#define putchar_(x) putc_((x), stdout_)

void debug(FILE_ *fp)
{
  printf("(debug) fp->cnt = %d\n", fp->cnt);
  printf("(debug) fp->ptr = %p\n", fp->ptr);
  printf("(debug) fp->base = %p\n", fp->base);
  printf("(debug) fp->flag = 0x%x\n", fp->flag);
  printf("(debug) fp->fd = %d\n", fp->fd);
}

FILE_ *fopen_(char *name, char *mode)
{
  int fd;
  FILE_ *fp;
  if (*mode != 'r' && *mode != 'w' && *mode != 'a')
    return NULL_;
  for (fp = _iob; fp < _iob + OPEN_MAX; ++fp) {
    if ((fp->flag & (_READ | _WRITE)) == 0)
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
  else {
    fd = open(name, O_RDONLY, 0);
  }
  if (fd == -1)
    return NULL_;
  fp->fd = fd;
  fp->cnt = 0;
  fp->base = NULL_;
  fp->flag = (*mode == 'r') ? _READ : _WRITE;
  printf("(debug) fopen_(name=%s)\n", name);
  debug(fp);
  return fp;
}

void fclose_(FILE_ *fp)
{
  if (fp->cnt != 0)
    fflush_(fp);
  if (fp->base != NULL_) {
    free(fp->base);
    fp->base = NULL_;
  }
  fp->ptr = fp->base;
  fp->flag &= ~(_READ | _WRITE);
  printf("(debug) fclose_()\n");
  debug(fp);
}

int _fillbuf(FILE_ *fp)
{
  // printf("(debug) _fillbuf()\n");
  int bufsize;
  if ((fp->flag & (_READ | _EOF | _ERR)) != _READ)
    return EOF_;
  bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ_;
  if (fp->base == NULL_)
    if ((fp->base = (char *)malloc(bufsize)) == NULL_)
      return EOF_;
    else {
      printf("(debug) _fillbuf(): malloc success\n");
    }
  fp->ptr = fp->base;
  fp->cnt = read(fp->fd, fp->ptr, bufsize);
  // printf("(debug) _fillbuf(): fp->cnt = %d\n", fp->cnt);
  // printf("(debug) _fillbuf(): fp->ptr = %s\n", fp->ptr);
  if (--fp->cnt < 0) {
    if (fp->cnt == -1) {
      fp->flag |= _EOF;
    }
    else {
      fp->flag |= _ERR;
    }
    fp->cnt = 0;
    return EOF_;
  }
  // printf("(debug) _fillbuf(): fp->ptr = %s\n", fp->ptr);
  int rc = (unsigned char)*fp->ptr++;
  // printf("(debug) _fillbuf(): rc = %c\n", rc);
  return rc;
  // return (unsigned char)*fp->ptr++;
}

// because of definition of putc_, cnt means num of available empty slots
int _flushbuf(int c, FILE_ *fp)
{
  if ((fp->flag & (_WRITE | _EOF | _ERR)) != _WRITE)
    return EOF_;
  if (fp->flag & _UNBUF) {
    if (write(fp->fd, (unsigned char *)&c, 1) != 1) {
      fp->flag |= _ERR;
      return _EOF;
    }
    return c;
  }
  // printf("(debug) _flushbuf\n");
  if (fp->base == NULL_) {
    if ((fp->base = (char *)malloc(BUFSIZ_)) == NULL_)
      return EOF_;
    else {
      printf("(debug) _flushbuf(): malloc success\n");
    }
    fp->cnt = BUFSIZ_;
    fp->ptr = fp->base;
  }
  // printf("(debug) _flushbuf: debug fp\n");
  // debug(fp);
  if (fp->cnt < 0)
    fp->cnt = 0;
  int rc = write(fp->fd, fp->base, BUFSIZ_ - fp->cnt);
  // printf("(debug) _flushbuf(): rc = %d\n", rc);
  if (rc == -1 || rc != BUFSIZ_ - fp->cnt) {
    // printf("(debug) _flushbuf(): write failure\n");
    fp->flag |= _ERR;
    return EOF_;
  }
  else {
    // printf("(debug) _flushbuf(): write success\n");
  }
  // fp->cnt += rc;
  // while (rc-- > 0)
  //   --fp->ptr;
  // --fp->cnt;
  fp->cnt = BUFSIZ_ - 1;
  fp->ptr = fp->base;
  return *fp->ptr++ = c;
}

int fflush_(FILE_ *fp)
{
  int rc = 0;
  if (fp < _iob || fp >= _iob + OPEN_MAX)
    return EOF;
  if (fp->flag & _WRITE) {
    rc = _flushbuf('\0', fp);
  }
  return rc;
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
  printf("(debug) main(): start get\n");
  int c;
  while ((c = getc_(fp)) != EOF) {
    // putc_(c, stderr_);
    putchar_(c);
  }
  _flushbuf('\0', stdout_);
  fclose_(fp);
  return 0;
}
