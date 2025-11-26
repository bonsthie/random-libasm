
section .text
global __strlen_base:hidden

; size_t __strlen_base(const char *str)
; str -> rdi
__strlen_base:
    mov     rax, rdi ; rax -> endptr

.loop:
	cmp byte [rax], 0
	je	.end
	inc rax
	jmp .loop

.end:
	sub	rax, rdi
	ret
