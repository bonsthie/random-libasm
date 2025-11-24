

; for now i need only those one but might change in the future
struc cpu_info_s
	.sse resb 1
	.avx2 resb 1
	.avx512f resb 1
endstruc

%define OFFSET(x) (1 << x)

%define CPU_INFO_SSE_BIT OFFSET(0)
%define CPU_INFO_AVX2_BIT OFFSET(1)
%define CPU_INFO_AVX512_BIT OFFSET(2)


%define CPU_INFO(field) __cpu_info + cpu_info_s.%+field

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; CPUID LEAF 1 (EAX=1)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; ECX bits
%define BIT_SSE3       (1 << 0)
%define BIT_SSSE3      (1 << 9)
%define BIT_FMA        (1 << 12)
%define BIT_SSE4_1     (1 << 19)
%define BIT_SSE4_2     (1 << 20)
%define BIT_XSAVE      (1 << 26)
%define BIT_OSXSAVE    (1 << 27)
%define BIT_AVX        (1 << 28)

; EDX bits
%define BIT_MMX        (1 << 23)
%define BIT_SSE        (1 << 25)
%define BIT_SSE2       (1 << 26)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; CPUID LEAF 7, SUBLEAF 0 (EAX=7, ECX=0)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; EBX bits
%define BIT_ERMS         (1 << 9)
%define BIT_AVX2         (1 << 5)
%define BIT_AVX512F      (1 << 16)
%define BIT_AVX512DQ     (1 << 17)
%define BIT_AVX512IFMA   (1 << 21)
%define BIT_AVX512PF     (1 << 26)
%define BIT_AVX512ER     (1 << 27)
%define BIT_AVX512CD     (1 << 28)
%define BIT_AVX512BW     (1 << 30)
%define BIT_AVX512VL     (1 << 31)

; 1 -> res bit
; 2 -> reg of the cpuid
; 3 -> bit to check
%macro HAS_CPU_FEAT 3
	test		%2, %3

	mov			al, [%1]
	mov			byte [%1], al

%%has_cpu_feat.end:
%endmacro
