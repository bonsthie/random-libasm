
#include <stdio.h>
#include <stdlib.h>
char *ft_strdup(const char *);

int main(int ac, char **av) {

  if (ac == 2) {
    char *str = ft_strdup(av[1]);
    printf("%s\n", str);
	free(str);
  }
  return 0;
}
