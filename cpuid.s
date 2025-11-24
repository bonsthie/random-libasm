%include "cpuid.inc"

section .bss
align 8
__cpu_info resb cpu_info_s_size

global __cpu_info:

section .init
global __get_cpu_feature:

__get_cpu_feature:
	mov eax, 1 ; leaf eax = 1
	cpuid
	; HAS_CPU_FEAT	__cpu_info_s.sse, edx, BIT_SSE

	CPU_INFO(sse)

	mov			al, [rel __cpu_info]
	mov			r8b, 1
	shl			r8b, CPU_INFO_SSE_BIT 
	or			al, r8b
	mov			[rel __cpu_info], al

	ret



