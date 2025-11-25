#include <string.h>
#include <stdio.h>
int __strcmp_base(const char *s1, const char *s2);

int main(int ac, char **av) {
	if (ac != 3)
		return 1;
	printf("%d %d\n", __strcmp_base(av[1], av[2]), strcmp(av[1], av[2]));
}
