#include <fcntl.h>    // for creat
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSIZ_  10
#define EOF_     (-1)
#define MAXLINE  1000
#define NULL_    0
#define MAXOPEN 20
#define PERMS    0666

enum _flags { READ_ = 01, WRITE_ = 02, UNBUF_ = 04, EOF_ = 010, ERR_ = 020 };
typedef struct _iobuf
{
  int cnt;
  char *ptr;
  char *base;
  int flag;
  int fd;
} FILE_;
FILE_ _iob[MAXOPEN] = { { 0, NULL_, NULL_, READ_, 0 },
                         { 0, NULL_, NULL_, WRITE_, 1 },
                         { 0, NULL_, NULL_, WRITE_ | UNBUF_, 2 } };
#define stdin_  (&_iob[0])
#define stdout_ (&_iob[1])
#define stderr_ (&_iob[2])


int _fillbuf(FILE_ *);
int _flushbuf(int, FILE_ *);
int fflush_(FILE_ *fp);


#define feof(p)   ((p)->flag.is_eof)
#define ferror(p) ((p)->flag.is_err)
#define fileno(p) ((p)->fd)

// #define getc_(p)    (--(p)->cnt >= 0 ? (unsigned char)*(p)->ptr++ : _fillbuf(p))
#define getc_(p) ((fp)->cnt ? (--(fp)->cnt, (unsigned char)*(fp)->ptr++) : _refill(fp))
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

// #include <sys/stat.h>
// #include <fcntl.h>

// int open(const char *path, int flags, mode_t mode);
// int creat(const char *path, mode_t mode);

// creat(path, mode) is equivalent to
//    open(path, O_CREAT | O_TRUNC | O_WRONLY, mode);

// #include <unistd.h>

// off_t lseek(int fd, off_t offset, int whence);

// note: l means "long integer" in lseek, whence means "from where/which"

// whence
// ------
// SEEK_SET
// SEEK_CUR
// SEEK_END

// return value
// ------------
// file pointer   success
// NULL_          failure

#define SEEK_SET_ 0
#define SEEK_CUR_ 1
#define SEEK_END_ 2

FILE_ *fopen_(const char *name, const char *mode)
{
  int fd;
  FILE_ *fp;
  if (*mode != 'r' && *mode != 'w' && *mode != 'a')
    return NULL_;
  // find a free slot
  for (fp = _iob; fp != _iob + MAXOPEN; ++fp) {
    if ((fp->flag & (READ_ | WRITE_)) == 0)
      break;
  }
  // no free slots
  if (fp == _iob + MAXOPEN)
    return NULL_;
  // open for read
  if (*mode == 'r') {
    if ((fd = open(name, O_RDONLY, 0)) == -1)
      return NULL_;
  }
  // open for write
  else if (*mode == 'w') {
    if ((fd = creat(name, PERMS)) == -1) // anyone can read and write
      return NULL_;
  }
  // open for append
  else {
    if ((fd = open(name, O_WRONLY, 0)) == -1) // 3rd arg for newly created file
      if ((fd = creat(name, PERMS)) == -1)
        return NULL_;
    if (lseek(fd, 0L, SEEK_END_) == -1)
      return NULL_;
  }
  fp->fd = fd;
  fp->cnt = 0;
  fp->base = NULL_;
  fp->flag = (*mode == 'r') ? READ_ : WRITE_;
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
  fp->flag &= ~(READ_ | WRITE_);
  printf("(debug) fclose_()\n");
  debug(fp);
}




// linux
// #include <unistd.h>

// ssize_t read(int fd, void *buf, size_t count);

// BSD
// #include <sys/types.h>
// #include <sys/uio.h>
// #include <unistd.h>

// ssize_t read(int fildes, void *buf, size_t nbyte);

// as a return for getc_
int _refill(FILE_ * fp)
{
  if ((fp->flag & (READ_ | EOF_ | ERR_)) != READ_)
    return EOF_;
  int bufsize = (fp->flag & UNBUF_) ? 1 : BUFSIZ_;
  // init buffer
  if (fp->base == NULL_) {
    if ((fp->base = (char *)malloc(bufsize)) == NULL_)
      return EOF_;
  }
  fp->ptr = fp->base;
  ssize_t rc = read(fp->fd, fp->ptr, bufsize);
  if (rc == 0) {
    fp->flag |= EOF_;
    return EOF_;
  }
  if (rc == -1) {
    fp->flag |= ERR_;
    return EOF_;
  }
  fp->cnt = rc - 1;
  return (unsigned char)*fp->ptr++;
}
// 写到这里，下次继续

// int _fillbuf(FILE_ *fp)
// {
//   // printf("(debug) _fillbuf()\n");
//   int bufsize;
//   if ((fp->flag & (READ_ | EOF_ | ERR_)) != READ_)
//     return EOF_;
//   bufsize = (fp->flag & UNBUF_) ? 1 : BUFSIZ_;
//   if (fp->base == NULL_)
//     if ((fp->base = (char *)malloc(bufsize)) == NULL_)
//       return EOF_;
//     else {
//       printf("(debug) _fillbuf(): malloc success\n");
//     }
//   fp->ptr = fp->base;
//   fp->cnt = read(fp->fd, fp->ptr, bufsize);
//   // printf("(debug) _fillbuf(): fp->cnt = %d\n", fp->cnt);
//   // printf("(debug) _fillbuf(): fp->ptr = %s\n", fp->ptr);
//   if (--fp->cnt < 0) {
//     if (fp->cnt == -1) {
//       fp->flag |= EOF_;
//     }
//     else {
//       fp->flag |= ERR_;
//     }
//     fp->cnt = 0;
//     return EOF_;
//   }
//   // printf("(debug) _fillbuf(): fp->ptr = %s\n", fp->ptr);
//   int rc = (unsigned char)*fp->ptr++;
//   // printf("(debug) _fillbuf(): rc = %c\n", rc);
//   return rc;
//   // return (unsigned char)*fp->ptr++;
// }



// because of definition of putc_, cnt means num of available empty slots
int _flushbuf(int c, FILE_ *fp)
{
  if ((fp->flag & (WRITE_ | EOF_ | ERR_)) != WRITE_)
    return EOF_;
  if (fp->flag & UNBUF_) {
    if (write(fp->fd, (unsigned char *)&c, 1) != 1) {
      fp->flag |= ERR_;
      return EOF_;
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
    fp->flag |= ERR_;
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
  if (fp < _iob || fp >= _iob + MAXOPEN)
    return EOF;
  if (fp->flag & WRITE_) {
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
