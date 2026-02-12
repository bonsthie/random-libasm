#ifndef __TEST_TEMPALTE_H__
#define __TEST_TEMPALTE_H__

#include <colors.h>
#include <stdbool.h>

bool tester_is_verbose(void);

#ifdef NO_TEST_EXIT
#define TEST_EXIT
#else
#define TEST_EXIT exit(1);
#endif

#define TEST(description, func, cmp_res_func, expected, args...)               \
  {                                                                            \
    long __expected = (long)expected;                                          \
    long result = (long)func(args);                                            \
    if (cmp_res_func((long)expected, (long)result)) {                          \
      test_success();                                                          \
      if (tester_is_verbose()) {                                               \
        printf(GREEN "[PASS]" RESET " %s => expected: %ld, got: %ld\n",        \
               description, __expected, result);                                 \
      }                                                                        \
    } else {                                                                   \
      test_fail();                                                             \
      printf(RED "[FAIL]" RESET " %s => expected: %ld, got: %ld\n",            \
             description, __expected, result);                                   \
      TEST_EXIT                                                                \
    }                                                                          \
  }

#define BASIC_CMP_RES_FUNC(res, func_res) (res == func_res)
#define BASIC_CMP_RES_FUNC_ADDR(res, func_res) (res == &func_res)
#define BASIC_CMP_RES_FUNC_PTR_VALUE(res, func_res) (res == *func_res)
#define ERRNO_CMP(expected, _) (expected == errno)

#define BASIC_TEST_ADDRESS(description, func, ret, args...)                    \
  TEST(description, func, BASIC_CMP_RES_FUNC_ADDR, ret, args)

#define BASIC_TEST_PTR_VALUE(description, func, ret, args...)                  \
  TEST(description, func, BASIC_CMP_RES_FUNC_PTR_VALUE, ret, args)

#define BASIC_TEST(description, func, ret, args...)                            \
  TEST(description, func, BASIC_CMP_RES_FUNC, ret, args)

#define TEST_ERRNO(description, func, errno_value, args...)                    \
  (description, func, ERRNO_CMP, ret, args)

#define STR_1000_a                                                             \
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" \
  "aaaaaaaaaaaaaaaaaa"                                                         \
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" \
  "aaaaaaaaaaaaaaaaaa"                                                         \
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" \
  "aaaaaaaaaaaaaaaaaa"                                                         \
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" \
  "aaaaaaaaaaaaaaaaaa"                                                         \
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" \
  "aaaaaaaaaaaaaaaaaa"                                                         \
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" \
  "aaaaaaaaaaaaaaaaaa"                                                         \
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" \
  "aaaaaaaaaaaaaaaaaa"                                                         \
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" \
  "aaaaaaaaaaaaaaaaaa"                                                         \
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" \
  "aaaaaaaaaaaaaaaaaa"                                                         \
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" \
  "aaaaaaaaaaaaaaaaaa"                                                         \
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" \
  "aaaaaa"
#define STR_10000_a                                                            \
  STR_1000_a STR_1000_a STR_1000_a STR_1000_a STR_1000_a STR_1000_a STR_1000_a \
      STR_1000_a STR_1000_a STR_1000_a
#define STR_100000_a                                                           \
  STR_10000_a STR_10000_a STR_10000_a STR_10000_a STR_10000_a STR_10000_a      \
      STR_10000_a STR_10000_a STR_10000_a STR_10000_a

#endif /* __TEST_TEMPALTE_H__ */
