

#include "libasm_tester.h"
#include <stdio.h>

void run_tests(test_entry *tests, void(tester)(void *), int(is_runable)(int)) {
  for (test_entry *t = tests; t->func != NULL; t++) {
    if (is_runable(t->version_id)) {
      printf("\ntesting %s\n", t->name);
      tester(t->func);
    } else {
      printf("\nskiping test for %s\n", t->name);
    }
  }
}
