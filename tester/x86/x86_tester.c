
#include "libasm_tester.h"
#include "x86_test_entry.h"
#include <stdio.h>

int main(int ac, char **av) {
  if (ac >= 2) {
    run_test_from_name(av[1]);
  } else {
    x86_string_test();
	x86_io_test();
  }

  tester_config *config = tester_get_config();

  printf("\ntest success %d\ntest fail %d\n", config->test_success,
         config->test_fail);
}
