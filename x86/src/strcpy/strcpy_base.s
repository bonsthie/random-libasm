
section .text
global __strcpy_base:hidden

; char	*__strcpy_base(char *dest, char *src)
; rdi -> dest
; rsi -> src
__strcpy_base:
	mov		rax, rdi

.loop:
	mov		dl, byte [rsi]
	test	dl, dl
	jz		.end
	mov		byte [rdi], dl
	inc		rsi
	inc		rdi
	jmp		.loop

.end:
	mov		byte [rdi], 0
	ret
	
