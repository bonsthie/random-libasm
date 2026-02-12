#include "x86_simd_level.h"
#include <libasm_tester.h>

#include "x86_simd_level.h"
#include "x86_test_entry.h"
#include <stddef.h>
#include <stdio.h>
#include <sys/cdefs.h>

size_t __strlen_base(const char *str);
size_t __strlen_sse2(const char *str);
size_t __strlen_avx2(const char *str);
size_t __strlen_avx512(const char *str);

void x86_strlen_test() {
  test_entry entry[] = {TEST_ENTRY(BASE, strlen), TEST_ENTRY(SSE2, strlen),
                        TEST_ENTRY(AVX2, strlen), TEST_ENTRY(AVX512F, strlen),
                        END_TEST_ENTRY};

  run_tests(entry, strlen_test, x86_is_runable);
}

int __strcmp_base(const char *s1, const char *s2);

void x86_strcmp_test() {
  test_entry entry[] = {TEST_ENTRY(BASE, strcmp), END_TEST_ENTRY};

  run_tests(entry, strcmp_test, x86_is_runable);
}

char *__strcpy_base(char *s1, char *s2);

void x86_strcpy_test() {
  test_entry entry[] = {TEST_ENTRY(BASE, strcpy), END_TEST_ENTRY};

  run_tests(entry, strcpy_test, x86_is_runable);
}

void x86_string_test() {
#define ENTRY(x) X86_ENTRY(x)();
#define GROUPS_ENTRY(x)
#include "string_entry.def"
}
