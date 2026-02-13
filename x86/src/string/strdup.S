

section .text
gloabl __strdup:hidden

; char *strdup(const char *s);
; s -> rdi
__strdup:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16


	add		rsp, 16
	pop		rbp
	ret

__strdup_opt:
	push	rax
	call	ft_strlen
	cmp		eax,eax

	mov		rdi, rax
	call	malloc	wrt..plt

