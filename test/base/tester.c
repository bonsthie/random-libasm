#include "libasm_tester.h"
#include <stdio.h>

static const tester_config g_tester_config = {
    .verbose = true,
    .arch_version = TESTER_ARCH_VERSION_ALL,
};

const tester_config *tester_get_config(void) { return &g_tester_config; }

bool tester_is_verbose(void) {
  const tester_config *config = tester_get_config();
  return config && config->verbose;
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
      if (tester_is_verbose()) {
        printf("\nskiping test for %s\n", t->name);
      }
      continue;
    }

    if (tester_is_verbose()) {
      printf("\ntesting %s\n", t->name);
    }
    tester(t->func);
  }
}
