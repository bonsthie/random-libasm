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

void strcmp_test() {
  printf("\ntesting strcmp...\n");
  BASIC_TEST("equal strings", strcmp, 0, "hello", "hello");
  BASIC_TEST("prefix match", strcmp, 'a' - '\0', "a", "");
  BASIC_TEST("first is smaller", strcmp, -1, "abc", "abd");
  BASIC_TEST("first is greater", strcmp, 2, "abe", "abc");
  BASIC_TEST("case sensitive", strcmp, 'a' - 'A', "abc", "Abc");
  BASIC_TEST("empty vs non-empty", strcmp, -'h', "", "hello");
  BASIC_TEST("non-empty vs empty", strcmp, 'h', "hello", "");
  BASIC_TEST("null bytes ignored", strcmp, 0, "abc\0xxx", "abc");
  BASIC_TEST("same long string", strcmp, 0, STR_10000_a, STR_10000_a);
  BASIC_TEST("long string with a bad char", strcmp, 1,
             STR_10000_a "b" STR_10000_a, STR_10000_a "a");
}

// void strncpy_test() {
//     printf("\ntesting strncpy...\n");
//     char buf[10] = {0};
//
//     strncpy(buf, "abc", 5);
//     BASIC_TEST("copy short string with padding", strcmp, 0, buf, "abc");
//
//     strncpy(buf, "hello world", 5);
//     buf[5] = '\0';
//     BASIC_TEST("copy truncated string", strcmp, 0, buf, "hello");
//
//     strncpy(buf, "", 5);
//     BASIC_TEST("copy empty string", strcmp, 0, buf, "");
// }


