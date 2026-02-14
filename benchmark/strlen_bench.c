#define _POSIX_C_SOURCE 200809L

#include <cpuid.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ARRAY_LEN(x) (sizeof(x) / sizeof((x)[0]))

extern size_t __strlen_base(const char *str);
extern size_t __strlen_sse2(const char *str);
extern size_t __strlen_avx2(const char *str);
extern size_t __strlen_avx512(const char *str);

enum feature_mask
{
    FEATURE_NONE = 0u,
    FEATURE_SSE2 = 1u << 0,
    FEATURE_AVX2 = 1u << 1,
    FEATURE_AVX512 = 1u << 2,
};

static double diff_ns(struct timespec start, struct timespec end)
{
    const double secs = (double)(end.tv_sec - start.tv_sec);
    const double nsecs = (double)(end.tv_nsec - start.tv_nsec);
    return secs * 1e9 + nsecs;
}

static double run_benchmark(const char *label,
                            size_t (*fn)(const char *),
                            const char *buffer,
                            size_t len,
                            size_t iterations)
{
    struct timespec start = {0}, end = {0};
    volatile size_t guard = 0;

    for (size_t i = 0; i < 128; ++i)
    {
        guard += fn(buffer);
    }

    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0)
    {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < iterations; ++i)
    {
        guard += fn(buffer);
    }

    if (clock_gettime(CLOCK_MONOTONIC, &end) != 0)
    {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }

    const double total_ns = diff_ns(start, end);
    const double per_call = total_ns / (double)iterations;

    printf("%-18s len=%7zu bytes -> %9.2f ns/call\n", label, len, per_call);

    guard += guard == 0;
    (void)guard;

    return per_call;
}

static unsigned detect_feature_mask(void)
{
    unsigned mask = FEATURE_NONE;

#if defined(__x86_64__) || defined(__i386__)
    __builtin_cpu_init();

    if (__builtin_cpu_supports("sse2"))
    {
        mask |= FEATURE_SSE2;
    }
    if (__builtin_cpu_supports("avx2"))
    {
        mask |= FEATURE_AVX2;
    }
    if (__builtin_cpu_supports("avx512f"))
    {
        mask |= FEATURE_AVX512;
    }
#endif

    return mask;
}

static int is_feature_supported(unsigned supported_mask, unsigned required_mask)
{
    return required_mask == FEATURE_NONE || ((supported_mask & required_mask) == required_mask);
}

int main(void)
{
    const size_t lengths[] = {16, 32, 64, 128, 256, 512, 1024, 4096, 16384, 65536};
    const size_t length_count = ARRAY_LEN(lengths);
    const size_t total_bytes_target = 512ull * 1024ull * 1024ull;
    const size_t alignment = 64;
    const size_t max_len = lengths[length_count - 1];

    char *buffer = NULL;
    if (posix_memalign((void **)&buffer, alignment, max_len + alignment) != 0)
    {
        fputs("Failed to allocate aligned buffer\n", stderr);
        return EXIT_FAILURE;
    }

    memset(buffer, 'A', max_len + alignment);

    struct
    {
        const char *name;
        size_t (*fn)(const char *);
        unsigned required_feature;
    } tests[] = {
        {"__strlen_base", __strlen_base, FEATURE_NONE},
        {"__strlen_sse2", __strlen_sse2, FEATURE_SSE2},
        {"__strlen_avx2", __strlen_avx2, FEATURE_AVX2},
        {"__strlen_avx512", __strlen_avx512, FEATURE_AVX512},
    };
    const size_t test_count = ARRAY_LEN(tests);

    const unsigned supported_features = detect_feature_mask();
    int enabled[ARRAY_LEN(tests)] = {0};
    for (size_t t = 0; t < test_count; ++t)
    {
        enabled[t] = is_feature_supported(supported_features, tests[t].required_feature);
        if (!enabled[t])
        {
            printf("Skipping %s benchmarks (unsupported CPU feature)\n", tests[t].name);
        }
    }
    putchar('\n');

    puts("strlen benchmark comparing asm implementations");
    puts("(ns/call, lower is better)\n");

    for (size_t i = 0; i < length_count; ++i)
    {
        const size_t len = lengths[i];
        size_t iterations = total_bytes_target / len;
        if (iterations < 1)
        {
            iterations = 1;
        }

        buffer[len - 1] = 'A';
        buffer[len] = '\0';

        double times[ARRAY_LEN(tests)] = {0};
        for (size_t t = 0; t < test_count; ++t)
        {
            if (!enabled[t])
            {
                continue;
            }

            times[t] = run_benchmark(tests[t].name,
                                     tests[t].fn,
                                     buffer,
                                     len,
                                     iterations);
        }

        for (size_t t = 1; t < test_count; ++t)
        {
            if (!enabled[t])
            {
                continue;
            }
            const double ratio = times[0] / times[t];
            printf("len=%7zu bytes -> ratio %s/%s = %.2fx\n",
                   len,
                   tests[0].name,
                   tests[t].name,
                   ratio);
        }
        putchar('\n');
    }

    free(buffer);
    return EXIT_SUCCESS;
}
