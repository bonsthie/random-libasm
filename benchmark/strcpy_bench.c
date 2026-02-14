#define _POSIX_C_SOURCE 200809L

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ARRAY_LEN(x) (sizeof(x) / sizeof((x)[0]))

extern char *__strcpy_base(char *dst, const char *src);
extern char *__strcpy_erms(char *dst, const char *src);

static double diff_ns(struct timespec start, struct timespec end)
{
    const double secs = (double)(end.tv_sec - start.tv_sec);
    const double nsecs = (double)(end.tv_nsec - start.tv_nsec);
    return secs * 1e9 + nsecs;
}

static double run_benchmark(const char *label,
                            char *(*fn)(char *, const char *),
                            char *dst,
                            const char *src,
                            size_t len,
                            size_t iterations)
{
    struct timespec start = {0}, end = {0};
    volatile char *guard = dst;

    for (size_t i = 0; i < 128; ++i)
    {
        guard = fn(dst, src);
    }

    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0)
    {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < iterations; ++i)
    {
        guard = fn(dst, src);
    }

    if (clock_gettime(CLOCK_MONOTONIC, &end) != 0)
    {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }

    const double total_ns = diff_ns(start, end);
    const double per_call = total_ns / (double)iterations;

    printf("%-18s len=%7zu bytes -> %9.2f ns/call\n", label, len, per_call);

    guard += guard == dst;
    (void)guard;

    return per_call;
}

int main(void)
{
    const size_t lengths[] = {16, 32, 64, 128, 256, 512, 1024, 4096, 16384, 65536};
    const size_t length_count = sizeof(lengths) / sizeof(lengths[0]);
    const size_t total_bytes_target = 512ull * 1024ull * 1024ull;
    const size_t alignment = 64;
    const size_t max_len = lengths[length_count - 1];

    char *src_buffer = NULL;
    char *dst_buffer = NULL;

    if (posix_memalign((void **)&src_buffer, alignment, max_len + alignment) != 0 ||
        posix_memalign((void **)&dst_buffer, alignment, max_len + alignment) != 0)
    {
        fputs("Failed to allocate aligned buffers\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < max_len + alignment; ++i)
    {
        src_buffer[i] = (char)('A' + (i % 23));
    }

    memset(dst_buffer, 0, max_len + alignment);

    struct
    {
        const char *name;
        char *(*fn)(char *, const char *);
    } tests[] = {
        {"__strcpy_base", __strcpy_base},
        {"__strcpy_erms", __strcpy_erms},
    };
    const size_t test_count = ARRAY_LEN(tests);

    puts("strcpy benchmark comparing base vs ERMS implementations");
    puts("(ns/call, lower is better)\n");

    for (size_t i = 0; i < length_count; ++i)
    {
        const size_t len = lengths[i];
        size_t iterations = total_bytes_target / len;
        if (iterations < 1)
        {
            iterations = 1;
        }

        src_buffer[len - 1] = (char)('A' + (len % 23));
        src_buffer[len] = '\0';

        double times[ARRAY_LEN(tests)] = {0};
        for (size_t t = 0; t < test_count; ++t)
        {
            times[t] = run_benchmark(tests[t].name,
                                     tests[t].fn,
                                     dst_buffer,
                                     src_buffer,
                                     len,
                                     iterations);
        }

        const double ratio = times[0] / times[1];
        printf("len=%7zu bytes -> ratio base/erms = %.2fx\n\n", len, ratio);
    }

    free(src_buffer);
    free(dst_buffer);

    return EXIT_SUCCESS;
}
