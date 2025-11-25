%include "cpuid.inc"

default rel

extern ft_write:hidden

section .bss
align 8
__cpu_info resb cpu_info_s_size


section .text
global __get_cpu_feature

__get_cpu_feature:
    push rbx ; rbx need to be preseved

	mov eax, 1			; leaf 1
	cpuid
	DETECT_CPU_FEAT	SSE, 1, edx
	DETECT_CPU_FEAT	SSE, 2, edx
	DETECT_CPU_FEAT	SSE, 4_1, ecx
	DETECT_CPU_FEAT	SSE, 4_2, ecx
	DETECT_CPU_FEAT	AVX, 1,	ecx
	
	mov eax, 7			; leaf 7
    xor ecx, ecx		; subleaf 0
	cpuid
	DETECT_CPU_FEAT	AVX, 2,	ebx
	DETECT_CPU_FEAT	AVX512, F, ebx

%ifdef VERBOSE
	call __print_cpu_feature
%endif
	pop rbx
	ret

%ifdef VERBOSE
__print_cpu_feature:
	PRINT_HAS_CPU_FEAT SSE, 1
	PRINT_HAS_CPU_FEAT SSE, 2
	PRINT_HAS_CPU_FEAT SSE, 4_1
	PRINT_HAS_CPU_FEAT AVX, 1
	PRINT_HAS_CPU_FEAT AVX, 2
	PRINT_HAS_CPU_FEAT AVX512, F
	ret

; void print_yes_or_no(bool yes);
; rdi -> 1 or 0
__print_yes_or_no:
	test	edi, edi
	jz		.no
	lea		rsi, [rel yes_msg]
	mov		edx, yes_len
	jmp		.end

.no:
	lea		rsi, [rel no_msg]
	mov		edx, no_len

.end:
	mov		edi, 1
	call ft_write
	ret


section .rodata
STRING has_SSE1, "has sse1: "
STRING has_SSE2, "has sse2: "
STRING has_SSE4_1, "has sse4_1: "
STRING has_SSE4_2, "has sse4_2: "
STRING has_AVX1, "has avx1: "
STRING has_AVX2, "has avx2: "
STRING has_AVX512F, "has avx512f: "

yes_msg:        db "yes", 10, 0
yes_len         equ $ - yes_msg

no_msg:         db "no", 10, 0
no_len          equ $ - no_msg
%endif

section .init_array
align 8
dq __get_cpu_feature

