#include "libasm_tester.h"
#include <colors.h>
#include <stdio.h>

static tester_config g_tester_config = {
    .verbose = true,
    .arch_version = TESTER_ARCH_VERSION_ALL,

};

tester_config *tester_get_config(void) { return &g_tester_config; }

bool tester_is_verbose(void) {
  const tester_config *config = tester_get_config();
  return config && config->verbose;
}

void test_success() {
  tester_config *config = tester_get_config();
  config->test_success++;
}

void test_fail() {
  tester_config *config = tester_get_config();
  config->test_success++;
}

void run_tests(test_entry *tests, void(tester)(void *), int(is_runable)(int)) {
  const tester_config *active_config = tester_get_config();
  const int arch_filter = active_config->arch_version;

  for (test_entry *t = tests; t->func != NULL; t++) {
    if (arch_filter != TESTER_ARCH_VERSION_ALL &&
        t->version_id != arch_filter) {
      continue;
    }

    if (!is_runable(t->version_id)) {
      printf("[" YELLOW "SKIPPING" RESET "] %s\n", t->name);
      continue;
    }

    printf("[" LGREEN "TESTING" RESET "]  %s\n", t->name);
    tester(t->func);
  }
}

void run_test(char *name, void *func, void(tester)(void *)) {
  printf("[" LGREEN "TESTING" RESET "]  %s\n", name);
  tester(func);
}
