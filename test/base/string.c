#include <test_template.h>
#include <string.h>

#include <libasm_tester.h>

void strlen_test(void *func) {
  strlen_proto strlen_tested = func;
  BASIC_TEST("simple short string", strlen_tested, 5, "test\n");
  BASIC_TEST("empty string", strlen_tested, 0, "");
  BASIC_TEST("string with only newline", strlen_tested, 1, "\n");
  BASIC_TEST("string with space", strlen_tested, 1, " ");
  BASIC_TEST("string with multiple words", strlen_tested, 11, "hello world");
  BASIC_TEST("string with tab character", strlen_tested, 4, "ab\tc");
  BASIC_TEST("string with special characters", strlen_tested, 6, "!@#$%^");
  BASIC_TEST("string with embedded null byte", strlen_tested, 3, "abc\0def"); // strlen_tested should stop at \0
  BASIC_TEST("string with numbers", strlen_tested, 3, "123");
  BASIC_TEST("string with full alphabet", strlen_tested, 26, "abcdefghijklmnopqrstuvwxyz");
  BASIC_TEST("string with leading null", strlen_tested, 0, "\0invisible");
  BASIC_TEST("medium string", strlen_tested, 57, "Lorem ipsum dolor sit amet, consectetur adipiscing elit.\n");
  BASIC_TEST("long string string", strlen_tested, 1000, STR_1000_a);
  BASIC_TEST("very long string string", strlen_tested, 10000, STR_10000_a);
  BASIC_TEST("very very long string string", strlen_tested, 100000, STR_100000_a);
  BASIC_TEST("UTF-8 multibyte characters (é)", strlen_tested, 2, "é");                                 // multibyte: 0xC3 0xA9
  BASIC_TEST("UTF-8 emoji (😀)", strlen_tested, 4, "😀"); // emoji is 4 bytes
}

void strcmp_test(void *func) {
  strcmp_proto strcmp_tested = func;
  BASIC_TEST("equal strings", strcmp_tested, 0, "hello", "hello");
  BASIC_TEST("prefix match", strcmp_tested, 'a' - '\0', "a", "");
  BASIC_TEST("first is smaller", strcmp_tested, -1, "abc", "abd");
  BASIC_TEST("first is greater", strcmp_tested, 2, "abe", "abc");
  BASIC_TEST("case sensitive", strcmp_tested, 'a' - 'A', "abc", "Abc");
  BASIC_TEST("empty vs non-empty", strcmp_tested, -'h', "", "hello");
  BASIC_TEST("non-empty vs empty", strcmp_tested, 'h', "hello", "");
  BASIC_TEST("null bytes ignored", strcmp_tested, 0, "abc\0xxx", "abc");
  BASIC_TEST("same long string", strcmp_tested, 0, STR_10000_a, STR_10000_a);
  BASIC_TEST("long string with a bad char", strcmp_tested, 1,
             STR_10000_a "b" STR_10000_a, STR_10000_a "a");
}


void strcpy_test(void *func) {
    strcpy_proto strcpy_tested = func;
    // 1. Simple short copy
    {
        char buf[16] = {0};
        strcpy_tested(buf, "abc");
        BASIC_TEST("copy short string", strcmp, 0, buf, "abc");
    }

    // 2. Copy empty string
    {
        char buf[16];
        memset(buf, 'X', sizeof(buf)); // ensure it really writes the '\0'
        strcpy_tested(buf, "");
        BASIC_TEST("copy empty string", strcmp, 0, buf, "");
    }

    // 3. Copy string that fits exactly (including '\0')
    {
        char buf[6] = {0}; // "hello" + '\0'
        strcpy_tested(buf, "hello");
        BASIC_TEST("copy exact-fit string", strcmp, 0, buf, "hello");
    }

    // 4. Overwrite previous content
    {
        char buf[16] = {0};
        strcpy_tested(buf, "first");
        strcpy_tested(buf, "second");
        BASIC_TEST("overwrite previous content", strcmp, 0, buf, "second");
    }

    // 5. Unaligned destination pointer
    {
        char storage[32];
        memset(storage, 0, sizeof(storage));
        char *buf = storage + 1;  // deliberately unaligned

        strcpy_tested(buf, "unaligned");
        BASIC_TEST("copy to unaligned destination", strcmp, 0, buf, "unaligned");
    }

    // 6. Long string copy
    {
        // reuse your big macro
        #define LONG_STR STR_10000_a "b"
        char buf[sizeof(LONG_STR)];
        memset(buf, 0, sizeof(buf));

        strcpy_tested(buf, LONG_STR);
        BASIC_TEST("copy long string", strcmp, 0, buf, LONG_STR);
        #undef LONG_STR
    }
}
