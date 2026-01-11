#include "libasm_tester.h"
#include "x86_test_entry.h"
#include <stdio.h>
#include <string.h>

#define ENTRY(x) void X86_ENTRY(x)();
#include "all_entry.def"
#undef ENTRY

#define ID(x) ID_X86_ENTRY(x)

enum TEST_ID {
#define ENTRY(x) ID(x),
#include "all_entry.def"
#undef ENTRY

      TEST_ID_COUNT
};

static const char *const test_id_names[] = {
#define ENTRY(x) #x,
#include "all_entry.def"
#undef ENTRY
};

static test_func tester[] = {
#define ENTRY(x) [ID(x)] = X86_ENTRY(x),
#include "all_entry.def"
#undef ENTRY

};

enum TEST_ID test_id_from_string(const char *s) {
  for (int i = 0; i < TEST_ID_COUNT; i++) {
    if (strcmp(s, test_id_names[i]) == 0)
      return (enum TEST_ID)i;
  }
  return -1; /* invalid */
}

void run_test_from_name(const char *s) {
	enum TEST_ID id = test_id_from_string(s);
	if ((int)id == -1) {
		printf("function %s dosen't have any test\n", s);
	}
	tester[id]();
}
