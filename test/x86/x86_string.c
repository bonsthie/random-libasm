
#include <libasm_tester.h>
#include "x86_simd_level.h"

#include "x86_simd_level.h"
#include <stddef.h>
#include <stdio.h>
#include <sys/cdefs.h>

size_t __strlen_base(const char *str);
size_t __strlen_sse2(const char *str);
size_t __strlen_avx2(const char *str);
size_t __strlen_avx512(const char *str);



static void x86_strlen_test() {
	printf("%p %p", __strlen_avx512, __strlen_base);
	test_entry entry[] = {
		TEST_ENTRY(BASE, strlen),
		TEST_ENTRY(SSE2, strlen),
		TEST_ENTRY(AVX2, strlen),
		TEST_ENTRY(AVX512F, strlen),
		END_TEST_ENTRY
	};

	run_tests(entry, strlen_test, x86_is_runable);
}

void x86_string_test() {
	x86_strlen_test();

}

int main() {
	x86_string_test();
}
