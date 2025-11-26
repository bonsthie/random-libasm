#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>


size_t __strlen_avx512(const char *); // your asm symbol


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
