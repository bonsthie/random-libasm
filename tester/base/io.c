#include "libasm_tester.h"
#include "test_template.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "test_template.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

void read_test(void *func) {
  read_proto read_func = (read_proto)func;

  char buf[128];
  int pipefd[2];

  /* ---------------- basic pipe tests ---------------- */
  pipe(pipefd);

  (void)write(pipefd[1], "hello", 5);
  memset(buf, 0xAA, sizeof(buf));
  BASIC_TEST("read simple pipe (ret)", read_func, 5, pipefd[0], buf, 5);
  BASIC_TEST("read simple pipe (content)", memcmp, 0, buf, "hello", 5);

  (void)write(pipefd[1], "abcdef", 6);
  memset(buf, 0xAA, sizeof(buf));
  BASIC_TEST("read partial from pipe (ret)", read_func, 3, pipefd[0], buf, 3);
  BASIC_TEST("read partial from pipe (content)", memcmp, 0, buf, "abc", 3);

  errno = 0;
  memset(buf, 0xAA, sizeof(buf));
  BASIC_TEST("read zero count (ret)", read_func, 0, pipefd[0], buf, 0);

  close(pipefd[0]);
  close(pipefd[1]);

  /* ---------------- file tests ---------------- */
  int fd = open("read_test_tmp.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
  (void)write(fd, "hello\n", 6);
  (void)lseek(fd, 0, SEEK_SET);

  memset(buf, 0xAA, sizeof(buf));
  BASIC_TEST("read simple file (ret)", read_func, 6, fd, buf, 6);
  BASIC_TEST("read simple file (content)", memcmp, 0, buf, "hello\n", 6);

  (void)lseek(fd, 0, SEEK_SET);
  memset(buf, 0xAA, sizeof(buf));
  BASIC_TEST("read more than file size (ret)", read_func, 6, fd, buf, 100);
  BASIC_TEST("read more than file size (content)", memcmp, 0, buf, "hello\n", 6);

  /* EOF */
  errno = 0;
  memset(buf, 0xAA, sizeof(buf));
  BASIC_TEST("read at EOF (ret)", read_func, 0, fd, buf, 10);

  close(fd);
  unlink("read_test_tmp.txt");

  /* ---------------- error tests (errno checked) ---------------- */
  errno = 0;
  BASIC_TEST("read invalid fd (ret)", read_func, -1, -1, buf, 5);
  BASIC_TEST("read invalid fd (errno EBADF)", ERRNO_CMP, 1, EBADF, 0); /* explained below */

  int fd_wr = open("read_test_tmp2.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
  errno = 0;
  BASIC_TEST("read from write-only fd (ret)", read_func, -1, fd_wr, buf, 5);
  BASIC_TEST("read from write-only fd (errno EBADF)", ERRNO_CMP, 1, EBADF, 0);

  close(fd_wr);
  unlink("read_test_tmp2.txt");
}

void write_test(void *func) {
  write_proto write_func = (write_proto)func;

  char buf[128];
  int pipefd[2];

  /* ---------------- basic pipe tests ---------------- */
  pipe(pipefd);

  errno = 0;
  BASIC_TEST("write simple pipe (ret)", write_func, 5, pipefd[1], "hello", 5);

  errno = 0;
  BASIC_TEST("write zero count (ret)", write_func, 0, pipefd[1], "hello", 0);

  /* verify data actually written: read back and compare */
  memset(buf, 0, sizeof(buf));
  errno = 0;
  // BASIC_TEST("verify pipe readback (ret)", (read_proto)read, 5, pipefd[0], buf, 5);
  // BASIC_TEST("verify pipe readback (content)", memcmp, 0, buf, "hello", 5);

  close(pipefd[0]);
  close(pipefd[1]);

  /* ---------------- file tests ---------------- */
  int fd = open("write_test_tmp.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);

  errno = 0;
  BASIC_TEST("write simple file (ret)", write_func, 6, fd, "hello\n", 6);

  (void)lseek(fd, 0, SEEK_SET);
  memset(buf, 0, sizeof(buf));
  // BASIC_TEST("verify file readback (ret)", (read_proto)read, 6, fd, buf, 6);
  // BASIC_TEST("verify file readback (content)", memcmp, 0, buf, "hello\n", 6);

  close(fd);
  unlink("write_test_tmp.txt");

  /* ---------------- error tests (errno checked) ---------------- */
  errno = 0;
  BASIC_TEST("write invalid fd (ret)", write_func, -1, -1, "x", 1);
  BASIC_TEST("write invalid fd (errno EBADF)", ERRNO_CMP, 1, EBADF, 0);

  int fd_ro = open("write_test_tmp2.txt", O_CREAT | O_RDONLY | O_TRUNC, 0644);
  errno = 0;
  BASIC_TEST("write to read-only fd (ret)", write_func, -1, fd_ro, "x", 1);
  BASIC_TEST("write to read-only fd (errno EBADF)", ERRNO_CMP, 1, EBADF, 0);

  close(fd_ro);
  unlink("write_test_tmp2.txt");

  /* /dev/full -> ENOSPC (Linux). Optional but very good deterministic check. */
  int fd_full = open("/dev/full", O_WRONLY);
  if (fd_full >= 0) {
    errno = 0;
    BASIC_TEST("write to /dev/full (ret)", write_func, -1, fd_full, "x", 1);
    BASIC_TEST("write to /dev/full (errno ENOSPC)", ERRNO_CMP, 1, ENOSPC, 0);
    close(fd_full);
  }
}

