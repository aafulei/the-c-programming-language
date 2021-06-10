/* Exercise 8-1. Rewrite the program cat from Chapter 7 using read, write,
 * open, and close instead of their standard library equivalents. Perform
 * experiments to determine the relative speeds of the two versions. */

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define STDIN 0
#define STDOUT 1
#define BUFSIZE 1000

void filecopy(int ifd, int ofd)
{
  char buf[BUFSIZE];
  int n;
  while ((n = read(ifd, buf, BUFSIZE)) > 0) {
    write(ofd, buf, n);
  }
}

int main(int argc, char *argv[])
{
  if (argc == 1)
    filecopy(STDIN, STDOUT);
  else {
    for (int i = 1, fd; i != argc; ++i) {
      if ((fd = open(argv[i], O_RDONLY,0)) == -1) {
        printf("cat: can't open %s\n", argv[i]);
        continue;
      }
      filecopy(fd, STDOUT);
      close(fd);
    }
  }
  return 0;
}
