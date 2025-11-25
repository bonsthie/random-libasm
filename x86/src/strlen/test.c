#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>


size_t __strlen_avx512(const char *); // your asm symbol

int page_overlap() {
  size_t page = sysconf(_SC_PAGESIZE);

  // Map two pages: [page0][page1]
  char *p = mmap(NULL, 2 * page, PROT_READ | PROT_WRITE,
                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (p == MAP_FAILED) {
    perror("mmap");
    return 1;
  }

  if (mprotect(p + page, page, PROT_NONE)) {
    perror("mprotect");
    return 1;
  }

  char *str = p + page - 8;
  {
    memcpy(str, "AAAAAA\0", 8);

    printf("str @ %p\n", (void *)str);

    size_t n = __strlen_avx512(str);
    printf("len = %zu\n", n);
  }

  if (mprotect(p + page, page, PROT_READ | PROT_WRITE)) {
    perror("mprotect");
    return 1;
  }

  {
	// because glibc memcpy segfault when starting between bage table
    memcpy(str, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\0", 100);

    printf("str @ %p\n", (void *)str);

    size_t n = __strlen_avx512(str);
    printf("big len = %zu\n", n);
  }

  return 0;
}

static void test_basic(void) {
  const char *tests[] = {"",
                         "a",
                         "hello",
                         "0123456789",
                         "short string",
                         "this is a longer string to test avx2 strlen",
                         NULL};

  printf("=== basic tests ===\n");
  for (int i = 0; tests[i]; ++i) {
    const char *s = tests[i];
    size_t glibc_len = strlen(s);
    size_t avx_len = __strlen_avx512(s);
    printf("  \"%s\": glibc=%zu, avx=%zu", s, glibc_len, avx_len);
    if (glibc_len != avx_len) {
      printf("  <-- MISMATCH\n");
    } else {
      printf("\n");
    }
  }
  printf("\n");
}

int main(void) {
  // test_basic();
  page_overlap();
}
