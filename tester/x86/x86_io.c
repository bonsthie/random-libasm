#include "libasm_tester.h"

ssize_t ft_write(int fildes, const void *buf, size_t nbyte);

void x86_write_test() {
	run_test("ft_write", ft_write, write_test);
}

ssize_t ft_read(int fildes, const void *buf, size_t nbyte);

void x86_read_test() {
	run_test("ft_read", ft_read, read_test);
}

void x86_io_test() {
	x86_write_test();
	x86_read_test();
}
