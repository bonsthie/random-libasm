#include <stddef.h>
size_t __strlen_avx512(const char *str);

int main(int ac, char **av) {
	if (ac ==2)
		return __strlen_avx512(av[1]);
	return 0;
}
