section .text
global __strcmp_base:hidden

; int __strcmp_base(const char *s1, const char *s2);
; s1 -> rdi
; s2 -> rsi
__strcmp_base:
	xor	edx, edx

.loop:
	movzx	eax, byte [rdi + rdx]
	movzx	ecx, byte [rsi + rdx]
	test	al, al
	jz		.end
	cmp		al, cl
	jne		.end
	inc		rdx
	jmp	.loop

.end:
	sub	eax, ecx
	ret
	

global ft_strcmp:weak
ft_strcmp equ __strcmp_base
