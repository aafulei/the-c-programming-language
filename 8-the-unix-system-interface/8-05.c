// note: cannot use system read() to read raw bytes of an arbitrary directory
// of dir, use readdir() see
// https://stackoverflow.com/questions/17618472/using-read-system-call-on-a-directory

/* Exercise 8-5. Modify the fsize program to print the other information
 * contained in the inode entry. */

#include <fcntl.h> /* flags for read and write */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define NAME_MAX 14
#define MAX_PATH 1024
#define DIR_SIZ_ 14

typedef struct
{
  long ino;
  char name[NAME_MAX + 1];
} Dirent_;

typedef struct
{
  int fd;
  Dirent_ d;
} DIR_;

struct direct_
{
  ino_t d_ino;
  char d_name[DIR_SIZ_];
};

// struct stat
// {
//   dev_t st_dev;
//   ino_t st_ino;
//   short st_mode;
//   short st_nlink;
//   short st_uid;
//   short st_gid;
//   dev_t st_rdev;
//   off_t st_size;
//   time_t st_atime;
//   time_t st_mtime;
//   time_t st_ctime;
// };

DIR_ *opendir_(char *dirname);
Dirent_ *readdir_(DIR_ *dfd);
void closedir_(DIR_ *dfd);
void dirwalk(char *, void (*fcn)(char *));
void fsize(char *);
int fstat(int fd, struct stat *);

/* closedir_: close directory opened by opendir_ */
void closedir_(DIR_ *dp)
{
  if (dp) {
    close(dp->fd);
    free(dp);
  }
}

DIR_ *opendir_(char *dirname)
{
  int fd;
  struct stat stbuf;
  DIR_ *dp;
  if ((fd = open(dirname, O_RDONLY, 0)) == -1 || fstat(fd, &stbuf) == -1
      || (stbuf.st_mode & S_IFMT) != S_IFDIR
      || (dp = (DIR_ *)malloc(sizeof(DIR_))) == NULL)
    return NULL;
  dp->fd = fd;
  return dp;
}

void dirwalk(char *dir, void (*fcn)(char *))
{
  char name[MAX_PATH];
  Dirent_ *dp;
  DIR_ *dfd;
  if ((dfd = opendir_(dir)) == NULL) {
    fprintf(stderr, "dirwalk: can't open %s\n", dir);
    return;
  }
  else {
    // fprintf(stderr, "(debug) dirwalk(): opened %s\n", dir);
  }

  // adapter begin
  DIR *real_dirp = opendir(dir);
  struct dirent *real_dp;
  while ((real_dp = readdir(real_dirp)) != NULL) {
    if (strcmp(real_dp->d_name, ".") == 0
        || strcmp(real_dp->d_name, "..") == 0) {
      // printf("(debug) skipped %s\n", real_dp->d_name);
      continue;
    }
    if (strlen(dir) + strlen(real_dp->d_name) + 2 > sizeof(name))
      fprintf(stderr, "dirwalk: name %s %s too long\n", dir, real_dp->d_name);
    else {
      sprintf(name, "%s/%s", dir, real_dp->d_name);
      (*fcn)(name);
    }
  }
  closedir(real_dirp);
  // adapter end

  closedir_(dfd);
}

void converttime(time_t *pt, char *buff)
{
  strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(pt));
}

void fsize(char *name)
{
  struct stat stbuf;
  // int stat(const char *path, struct stat *buf);
  if (stat(name, &stbuf) == -1) {
    fprintf(stderr, "fsize: can't access %s\n", name);
    return;
  }
  if ((stbuf.st_mode & S_IFMT) == S_IFDIR) {
    // fprintf(stderr, "(debug) %s is a dir\n", name);
    dirwalk(name, fsize);
  }
  else {
    // fprintf(stderr, "(debug) %s is not a dir\n", name);
  }
  char mtime[20];
  converttime(&stbuf.st_mtime, mtime);
  printf("size=%8ld | mode=%6o | uid = %d| mtime = %s | %s\n", stbuf.st_size,
         stbuf.st_mode, stbuf.st_uid, mtime, name);
}

int main(int argc, char **argv)
{
  if (argc == 1) /* default: current directory */
    fsize(".");
  else
    while (--argc > 0)
      fsize(*++argv);
  return 0;
}
