
#include "x86_test_entry.h"
int main(int ac, char **av) {
  if (ac >= 2) {
    run_test_from_name(av[1]);
  } else {
    x86_string_test();
  }
}
