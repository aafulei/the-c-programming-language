// 21/06/21 = Mon

// reinventing the wheel: buffered file functions in <stdio.h>

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>

#ifdef __APPLE__
#include <sys/types.h>
#include <sys/uio.h>
#endif

/* --- system calls -----------------------------------------------------------

  ====
  OPEN
  ====
  #include <fcntl.h>
  #include <sys/stat.h>

  int open(const char *path, int flags, mode_t mode);
  int creat(const char *path, mode_t mode);

  creat(path, mode) is equivalent to
     open(path, O_CREAT | O_TRUNC | O_WRONLY, mode);

  ====
  SEEK
  ====
  #include <unistd.h>
  off_t lseek(int fd, off_t offset, int whence);

  note: l means "long integer" in lseek, whence means "from where/which"

  whence
  ------
  SEEK_SET
  SEEK_CUR
  SEEK_END

  ====
  READ
  ====
  linux
  -----
  #include <unistd.h>

  ssize_t read(int fd, void *buf, size_t count);

  BSD
  ---
  #include <sys/types.h>
  #include <sys/uio.h>
  #include <unistd.h>

  ssize_t read(int fildes, void *buf, size_t nbyte);

--------------------------------------------------------------------------- */

#define BUFSIZ_ 1024
#define EOF_    (-1)
#define MAXLINE 1000
#define MAXOPEN 20
#define NULL_   0
#define PERMS   0666

#define STDIN_  0
#define STDERR_ 1
#define STDOUT_ 2

enum _flags {
  READ_FLAG = 0x1,
  WRITE_FLAG = 0x2,
  UNBUF_FLAG = 0x4,
  EOF_FLAG = 0x8,
  ERR_FLAG = 0x10
};

typedef struct
{
  int fd;
  int flag;
  char *base;
  int cnt;
  char *ptr;
} FILE_;

FILE_ _iob[MAXOPEN]
    = { { STDIN_, READ_FLAG, NULL_, 0, NULL_ },
        { STDOUT_, WRITE_FLAG, NULL_, 0, NULL_ },
        { STDERR_, WRITE_FLAG | UNBUF_FLAG, NULL_, 0, NULL_ } };

#define SEEK_SET_ 0
#define SEEK_CUR_ 1
#define SEEK_END_ 2

#define stdin_  (&_iob[0])
#define stdout_ (&_iob[1])
#define stderr_ (&_iob[2])

#define feof(fp)   ((fp)->flag.is_eof)
#define ferror(fp) ((fp)->flag.is_err)
#define fileno(fp) ((fp)->fd)

// clang-format off
#define getc_(fp)    ((fp)->flag & UNBUF_FLAG ? _getc_unbuf(fp) : _getc_buf(fp))
#define putc_(c, fp) ((fp)->flag & UNBUF_FLAG ? _putc_unbuf(c, fp) : _putc_buf(c, fp))
#define getchar_()   getc_(stdin_)
#define putchar_(c)  putc_((c), stdout_)
// clang-format on


FILE_ *fopen_(const char *name, const char *mode);
int fflush_(FILE_ *);
void fclose_(FILE_ *);

int _getc_buf(FILE_ *);
int _getc_unbuf(FILE_ *);
int _putc_unbuf(char, FILE_ *);
int _putc_buf(char, FILE_ *);
int _refill(FILE_ *);
int _flush(FILE_ *);
void _debug(FILE_ *);

// return value
// ------------
// on success         file pointer
// on failure         NULL_
FILE_ *fopen_(const char *name, const char *mode)
{
  int fd;
  FILE_ *fp;
  if (*mode != 'r' && *mode != 'w' && *mode != 'a')
    return NULL_;
  // find a free slot
  for (fp = _iob; fp != _iob + MAXOPEN; ++fp) {
    if ((fp->flag & (READ_FLAG | WRITE_FLAG)) == 0)
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
  fp->flag = (*mode == 'r') ? READ_FLAG : WRITE_FLAG;
  return fp;
}

// return value
// ------------
// on success         0
// on partial flush   number of characters unflushed in buffer
// on failure         EOF_
int fflush_(FILE_ *fp)
{
  if (!(fp->flag & WRITE_FLAG)) {
    fp->flag |= ERR_FLAG;
    return EOF_;
  }
  return _flush(fp);
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
  fp->flag &= ~(READ_FLAG | WRITE_FLAG);
}

// return value
// ------------
// on success         character (as unsigned char)
// on failure         EOF_
int _getc_buf(FILE_ *fp)
{
  if (fp->cnt == 0 && _refill(fp) == EOF_)
    return EOF_;
  --fp->cnt;
  return (unsigned char)*fp->ptr++;
}

// return value
// ------------
// on success         character (as unsigned char)
// on failure         EOF_
int _getc_unbuf(FILE_ *fp)
{
  char c;
  ssize_t rc = read(fp->fd, &c, 1);
  if (rc <= 0) {
    if (rc == -1)
      fp->flag |= ERR_FLAG;
    else
      fp->flag |= EOF_FLAG;
    return EOF_;
  }
  // casting to unsigned char to get rid of sign extension problems
  return (unsigned char)c;
}

// return value
// ------------
// on success         character (as unsigned char)
// on failure         EOF_
int _putc_unbuf(char c, FILE_ *fp)
{
  if (write(fp->fd, &c, 1) != 1) {
    fp->flag |= ERR_FLAG;
    return EOF_;
  }
  return (unsigned char)c;
}

// return value
// ------------
// on success         character (as unsigned char)
// on failure         EOF_
int _putc_buf(char c, FILE_ *fp)
{
  if (fp->cnt == 0 && _flush(fp) == EOF_)
    return EOF_;
  --fp->cnt;
  return (unsigned char)(*fp->ptr++ = c);
}

// return value
// ------------
// on success         0
// on failure         EOF_
int _refill(FILE_ *fp)
{
  if ((fp->flag & (READ_FLAG | UNBUF_FLAG | EOF_FLAG | ERR_FLAG)) != READ_FLAG)
    return EOF_;
  // init buffer
  if (!fp->base && !(fp->base = (char *)malloc(BUFSIZ_))) {
    fp->flag |= ERR_FLAG;
    return EOF_;
  }
  // read from fd to buffer
  fp->ptr = fp->base;
  ssize_t rc = read(fp->fd, fp->base, BUFSIZ_);
  if (rc <= 0) {
    // read error
    if (rc == -1)
      fp->flag |= ERR_FLAG;
    // read eof
    else
      fp->flag |= EOF_FLAG;
    fp->cnt = 0;
    return EOF_;
  }
  // read success
  else {
    fp->cnt = rc;
    return 0;
  }
}

// return value
// ------------
// on success         0
// on partial flush   number of characters unflushed in buffer
// on failure         EOF_
//
// note
// ----
// cnt is num of available empty slots
int _flush(FILE_ *fp)
{
  if ((fp->flag & (WRITE_FLAG | UNBUF_FLAG | EOF_FLAG | ERR_FLAG))
      != WRITE_FLAG)
    return EOF_;
  // init buffer
  ssize_t rest = 0;
  if (!fp->base) {
    if (!(fp->base = (char *)malloc(BUFSIZ_))) {
      fp->flag |= ERR_FLAG;
      return EOF_;
    }
    fp->cnt = BUFSIZ_;
  }
  // write from buffer to fd
  else {
    ssize_t rc = write(fp->fd, fp->base, BUFSIZ_ - fp->cnt);
    // in theory write never returns 0, it either returns a positive or -1
    if (rc == -1 || rc == 0) {
      fp->flag |= ERR_FLAG;
      return EOF_;
    }
    rest = BUFSIZ_ - fp->cnt - rc;
    if (rest) {
      // move memory to fix buffer
      char *p = fp->base;
      char *q = fp->base + rc;
      ssize_t n = rest;
      while (n-- > 0)
        *p++ = *q++;
    }
    fp->cnt += rc;
  }
  fp->ptr = fp->base;
  return rest;
}

void _debug(FILE_ *fp)
{
  fprintf(stderr, "(debug) fp->fd = %d\n", fp->fd);
  fprintf(stderr, "(debug) fp->flag = 0x%x\n", fp->flag);
  fprintf(stderr, "(debug) fp->base = %p\n", fp->base);
  fprintf(stderr, "(debug) fp->ptr = %p\n", fp->ptr);
  fprintf(stderr, "(debug) fp->cnt = %d\n", fp->cnt);
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
  int c;
  while ((c = getc_(fp)) != EOF_) {
    putchar_(c);
  }
  fclose_(fp);
  fflush_(stdout_);
  return 0;
}
