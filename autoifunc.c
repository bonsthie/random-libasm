int __attribute__((target_clones("default", "sse", "avx2"))) ft_strlen(const char *str) {
	const char *endptr;

	while (*endptr)
		endptr++;
	return endptr - str;
}
