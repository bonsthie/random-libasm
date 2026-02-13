
%include "cpuid.inc"

section .text
default rel

extern __strlen_base:function
extern __strlen_sse2:function
extern __strlen_avx2:function
extern __strlen_avx512:function


global __strlen_ifunc:function
__strlen_ifunc:
    IFUNC_TRY_IMPL r11d, 0, AVX512, F
    IFUNC_TRY_IMPL r11d, 1, AVX,    2
    IFUNC_TRY_IMPL r11d, 2, SSE,    2
    mov r11d, 3 ; default

.ret:
	lea				rax, [rel __strlen_version_table]
	IFUNC_RET		qword [r11 * 8 + rax]

global ft_strlen:gnu_ifunc
ft_strlen equ __strlen_ifunc

section .data

__strlen_version_table:
dq __strlen_avx512
dq __strlen_avx2
dq __strlen_sse2
dq __strlen_base
