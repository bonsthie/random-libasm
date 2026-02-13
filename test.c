unsigned long ft_strlen(const char *);

int main(int ac, char **av) {
	if (ac == 2)
		return ft_strlen(av[1]);
	return 0;
}
