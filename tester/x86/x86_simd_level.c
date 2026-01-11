
#include "x86_simd_level.h"
#include <stdint.h>
#include <stdio.h>

x86_simd_support_t x86_simd_support = {
	// debug
	.avx = 1
};

static x86_simd_support_t x86_detect_cpu_features() {
  uint32_t eax, ebx, ecx, edx;

	
	x86_simd_support.cpu_id = 0;
  // CPUID with EAX=1: Processor Info and Feature Bits
  if (__get_cpuid__(1, &eax, &ebx, &ecx, &edx)) {
    x86_simd_support.mmx = edx & Bit_MMX ? 1 : 0;
    x86_simd_support.sse = edx & Bit_SSE ? 1 : 0;
    x86_simd_support.sse2 = edx & Bit_SSE2 ? 1 : 0;
    x86_simd_support.sse3 = ecx & Bit_SSE3 ? 1 : 0;
    x86_simd_support.ssse3 = ecx & Bit_SSSE3 ? 1 : 0;
    x86_simd_support.sse41 = ecx & Bit_SSE4_1 ? 1 : 0;
    x86_simd_support.sse42 = ecx & Bit_SSE4_2 ? 1 : 0;
    x86_simd_support.avx = ecx & Bit_AVX ? 1 : 0;
	x86_simd_support.erms = ecx & Bit_ERMS ? 1 : 0;
  }

  // CPUID with EAX=7, ECX=0: Extended Features
  if (__get_cpuid_count__(7, 0, &eax, &ebx, &ecx, &edx)) {
    x86_simd_support.avx2 = ebx & Bit_AVX2 ? 1 : 0;
    x86_simd_support.avx512f = ebx & Bit_AVX512F ? 1 : 0;
    x86_simd_support.avx512dq = ebx & Bit_AVX512DQ ? 1 : 0;
    x86_simd_support.avx512ifma = ebx & Bit_AVX512IFMA ? 1 : 0;
    x86_simd_support.avx512pf = ebx & Bit_AVX512PF ? 1 : 0;
    x86_simd_support.avx512er = ebx & Bit_AVX512ER ? 1 : 0;
    x86_simd_support.avx512cd = ebx & Bit_AVX512CD ? 1 : 0;
    x86_simd_support.avx512bw = ebx & Bit_AVX512BW ? 1 : 0;
    x86_simd_support.avx512vl = ebx & Bit_AVX512VL ? 1 : 0;
  }
	return x86_simd_support;
}

#ifdef DEBUG

void x86_print_features() {
    printf("simd support %d\n", x86_simd_support.cpu_id);
    if (x86_simd_support.mmx) printf("-mmmx ");
    if (x86_simd_support.sse) printf("-msse ");
    if (x86_simd_support.sse2) printf("-msse2 ");
    if (x86_simd_support.sse3) printf("-msse3 ");
    if (x86_simd_support.ssse3) printf("-mssse3 ");
    if (x86_simd_support.sse41) printf("-msse4.1 ");
    if (x86_simd_support.sse42) printf("-msse4.2 ");
    if (x86_simd_support.avx) printf("-mavx ");
    if (x86_simd_support.avx2) printf("-mavx2 ");
    if (x86_simd_support.avx512f) printf("-mavx512f ");
    if (x86_simd_support.avx512dq) printf("-mavx512dq ");
    if (x86_simd_support.avx512ifma) printf("-mavx512ifma ");
    if (x86_simd_support.avx512pf) printf("-mavx512pf ");
    if (x86_simd_support.avx512er) printf("-mavx512er ");
    if (x86_simd_support.avx512cd) printf("-mavx512cd ");
    if (x86_simd_support.avx512bw) printf("-mavx512bw ");
    if (x86_simd_support.avx512vl) printf("-mavx512vl ");
    if (x86_simd_support.erms) printf("-erms ");

    printf("\n");
}

#endif

void __attribute__((constructor)) x86_init_simd_support(void) {
    x86_detect_cpu_features();
#ifdef DEBUG
	x86_print_features();
#endif
}


int x86_is_runable(int version) {
	switch (version) {
		case SIMD_LVL_BASE: return 1;
		case SIMD_LVL_SSE1: return x86_simd_support.sse;
		case SIMD_LVL_SSE2: return x86_simd_support.sse2;
		case SIMD_LVL_SSE4_1: return x86_simd_support.sse41;
		case SIMD_LVL_SSE4_2: return x86_simd_support.sse42;
		case SIMD_LVL_AVX2: return x86_simd_support.avx2;
		case SIMD_LVL_AVX512F: return x86_simd_support.avx512f;
	}
	__builtin_unreachable();
}
