
section .text
global __strdup:hidden

extern ft_strlen:function
extern ft_strcpy:function
extern malloc:function

; char *strdup(const char *s);
; s -> rdi
__strdup:
; to store the string becasue that preserved and align the stack
	push	rbx 
	mov		rbx, rdi

	call	ft_strlen
	
	lea		rdi, [rax + 1]
	call	malloc		wrt ..plt
	test	rax, rax
	je		.bad_alloc

	mov		rdi, rax
	mov		rsi, rbx
	pop		rbx
	jmp		ft_strcpy

.bad_alloc:
	pop		rbx
	ret

global ft_strdup:weak
ft_strdup equ __strdup
