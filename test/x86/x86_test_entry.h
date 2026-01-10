#ifndef X86_TEST_ENTRY
#define X86_TEST_ENTRY

#define X86_ENTRY(x) x86_##x##_test
#define ID_X86_ENTRY(x) id_x86_##x##_test

void run_test_from_name(const char *s);

void x86_string_test();

#endif // X86_TEST_ENTRY
