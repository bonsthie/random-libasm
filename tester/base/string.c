
#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <test_template.h>
#include <libasm_tester.h>

static long run_strdup_case(strdup_proto func, const char *input,
                            const char *expected) {
  char *dup = func(input);
  if (!dup)
    return 0;

  long ok = dup != input && strcmp(dup, expected) == 0;
  free(dup);
  return ok;
}

static long run_strdup_keeps_source_intact(strdup_proto func, char *input,
                                           size_t len) {
  char snapshot[64];
  if (len > sizeof(snapshot))
    len = sizeof(snapshot);
  memcpy(snapshot, input, len);

  char *dup = func(input);
  if (!dup)
    return 0;

  dup[0] = (dup[0] == 'x') ? 'y' : 'x';
  long ok = memcmp(snapshot, input, len) == 0;
  free(dup);
  return ok;
}

static long run_strdup_two_alloc_case(strdup_proto func, const char *input) {
  char *dup1 = func(input);
  char *dup2 = func(input);
  if (!dup1 || !dup2) {
    free(dup1);
    free(dup2);
    return 0;
  }

  long ok = dup1 != dup2 && dup1 != input && dup2 != input &&
             strcmp(dup1, dup2) == 0;
  free(dup1);
  free(dup2);
  return ok;
}

void strlen_page_overlap(strlen_proto strlen_tested) {
  size_t page = sysconf(_SC_PAGESIZE);

  // Map two pages: [page0][page1]
  char *p = mmap(NULL, 2 * page, PROT_READ | PROT_WRITE,
                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (p == MAP_FAILED) {
    perror("mmap");
    return;
  }

  if (mprotect(p + page, page, PROT_NONE)) {
    perror("mprotect");
    return;
  }

  char *str = p + page - 8;
  memcpy(str, "AAAAAA\0", 7);
  BASIC_TEST("page overlap on the first 16 byte and end in the first page", strlen_tested, 6, str);

  if (mprotect(p + page, page, PROT_READ | PROT_WRITE)) {
    perror("mprotect");
    return;
  }

  memcpy(str, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\0", 100);
  BASIC_TEST("page overlap on the first 16 byte but that continue", strlen_tested, 99, str);
}

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

  strlen_page_overlap(strlen_tested);
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

void strdup_test(void *func) {
  strdup_proto strdup_tested = func;

  BASIC_TEST("duplicate simple string", run_strdup_case, 1, strdup_tested,
             "hello world", "hello world");

  BASIC_TEST("duplicate empty string", run_strdup_case, 1, strdup_tested, "",
             "");

  BASIC_TEST("duplicate ascii punctuation", run_strdup_case, 1, strdup_tested,
             "!@#$%^&*()_+", "!@#$%^&*()_+");

  BASIC_TEST("duplicate long string", run_strdup_case, 1, strdup_tested,
             STR_1000_a, STR_1000_a);

  BASIC_TEST("duplicate very long string", run_strdup_case, 1, strdup_tested,
             STR_10000_a, STR_10000_a);

  BASIC_TEST("duplicate utf-8 content", run_strdup_case, 1, strdup_tested,
             "😀éß", "😀éß");

  char embedded_null[] = "abcxyz";
  embedded_null[3] = '\0';
  BASIC_TEST("stops at first null byte", run_strdup_case, 1, strdup_tested,
             embedded_null, "abc");

  char mutable_src[] = "mutable buffer";
  BASIC_TEST("result is independent from source",
             run_strdup_keeps_source_intact, 1, strdup_tested, mutable_src,
             sizeof(mutable_src));

  BASIC_TEST("distinct buffers on consecutive calls",
             run_strdup_two_alloc_case, 1, strdup_tested, "duplicate twice");
}
