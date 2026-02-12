#include "libasm_tester.h"
#include "x86_test_entry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENTRY(x) void X86_ENTRY(x)();
#define GROUPS_ENTRY(x) ENTRY(x)
#include "all_entry.def"

#define ID(x) ID_X86_ENTRY(x)
enum X86_TEST_ID_FUNC {
#define ENTRY(x) ID(x),
#include "all_entry.def"

  TEST_ID_COUNT_FUNC
};

static const char *const test_id_names_func[] = {
#define ENTRY(x) #x,
#include "all_entry.def"
};

static test_func tester_func[] = {
#define ENTRY(x) [ID(x)] = X86_ENTRY(x),
#include "all_entry.def"

};

enum X86_TEST_ID_FUNC test_id_from_string_func(const char *s) {
  for (int i = 0; i < TEST_ID_COUNT_FUNC; i++) {
    if (strcmp(s, test_id_names_func[i]) == 0)
      return (enum X86_TEST_ID_FUNC)i;
  }
  return -1; /* invalid */
}

enum X86_TEST_ID_GROUPS {
#define GROUPS_ENTRY(x) ID(x),
#include "all_entry.def"

  TEST_ID_COUNT_GROUPS
};

static const char *const test_id_names_groups[] = {
#define GROUPS_ENTRY(x) #x,
#include "all_entry.def"
};

static test_func tester_groups[] = {
#define GROUPS_ENTRY(x) [ID(x)] = X86_ENTRY(x),
#include "all_entry.def"

};

enum X86_TEST_ID_GROUPS test_id_from_string_groups(const char *s) {
  for (int i = 0; i < TEST_ID_COUNT_GROUPS; i++) {
    if (strcmp(s, test_id_names_groups[i]) == 0)
      return (enum X86_TEST_ID_GROUPS)i;
  }
  return -1; /* invalid */
}

void x86_run_test_from_name(const char *s) {
  enum X86_TEST_ID_FUNC id = test_id_from_string_func(s);
  if ((int)id != -1) {
    tester_func[id]();
    return;
  }

  enum X86_TEST_ID_GROUPS id_group = test_id_from_string_groups(s);
  if ((int)id_group != -1) {
    tester_groups[id_group]();
    return;
  }

  printf("function %s dosen't have any test\n", s);
  exit(1);
}

void x86_run_all_groups() {
  for (int i = 0; i < TEST_ID_COUNT_GROUPS; i++) {
    tester_groups[i]();
  }
}
