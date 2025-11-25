
#include <stddef.h>
volatile int i = 1;

size_t ft_strlen(const char *str) __attribute__((ifunc("ft_strlen_campte")));

size_t	ft_strlen_0(const char *str)
{
	const char	*start;

	start = str;
	while (*start)
		++start;
	return (start - str);
}

size_t	ft_strlen_1(const char *str)
{
	const char	*start;

	start = str;
	while (*start)
		++start;
	return (start - str + 1);
}

void *ft_strlen_campte() {
	if (i == 1) {
		return ft_strlen_0;
	}
	return ft_strlen_1;
}


int main(int ac,char **av) {
	if (ac == 2)
		return ft_strlen(av[1]);
	return 0;
}
