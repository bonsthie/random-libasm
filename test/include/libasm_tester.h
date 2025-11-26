#ifndef __LIBASM_TESTER_H__
#define __LIBASM_TESTER_H__

#include <stddef.h>

typedef struct {
  const char *name;
  void *func;
  int version_id;
} test_entry;

#define END_TEST_ENTRY {.func = NULL}

#define TEST_ENTRY(version, function)                                              \
  {.func = F_##version(function),                                                  \
   .name = F_##version##_NAME(function),                                           \
   .version_id = SIMD_LVL_##version}

void run_tests(test_entry *tests, void(tester)(void *), int(is_runable)(int));

typedef size_t (*strlen_proto)(const char *str);
void strlen_test(void *func);

#endif // __LIBASM_TESTER_H__
