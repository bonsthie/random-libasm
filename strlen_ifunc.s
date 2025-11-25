extern __strlen_base
extern __strlen_avx2

section .text
global __strlen_ifunc

__strlen_ifunc:
	lea rax, [rel __strlen_base]
	ret
	

ft_strlen equ __strlen_ifunc
