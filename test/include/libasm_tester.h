#ifndef __LIBASM_TESTER_H__
#define __LIBASM_TESTER_H__

#include <stdbool.h>
#include <stddef.h>

typedef struct {
  const char *name;
  void *func;
  int version_id;
} test_entry;

typedef struct {
  bool verbose;

  // use to run only a specific version EX AVX SSE2..
  int arch_version;
} tester_config;

typedef void (*test_func)();

#define TESTER_ARCH_VERSION_ALL (-1)

#define END_TEST_ENTRY {.func = NULL}

#define TEST_ENTRY(version, function)                                          \
  {.func = F_##version(function),                                              \
   .name = F_##version##_NAME(function),                                       \
   .version_id = SIMD_LVL_##version}

void run_tests(test_entry *tests, void(tester)(void *), int(is_runable)(int));

void tester_set_config(const tester_config *config);
const tester_config *tester_get_config(void);
bool tester_is_verbose(void);

typedef size_t (*strlen_proto)(const char *str);
void strlen_test(void *func);

typedef int (*strcmp_proto)(const char *s1, const char *s2);
void strcmp_test(void *func);

typedef char *(*strcpy_proto)(char *dst, const char *src);
void strcpy_test(void *func);

#endif // __LIBASM_TESTER_H__
