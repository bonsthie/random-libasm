
section .text
global __strlen_avx512:hidden

; size_t __strlen_base(const char *str)
; str -> rdi
__strlen_avx512:
	mov			rax, rdi ; rax = endptr
	vpxorq		zmm0, zmm0, zmm0 ; zeros mask

	; is there a missing aligment
	mov			rsi, rdi
	and			rsi, 63
	jz			.loop_align

	; miss aligment offset in rcx
	mov			rcx, 64
	sub			rcx, rsi

	; first cmp unalign
	vmovdqu8	zmm1, [rax] 
	vpcmpeqb	k1, zmm1, zmm0
	kortestq	k1, k1
	jne			.end
	add			rax, rcx

.loop_align:
	vmovdqa64	zmm1, [rax] 
	vpcmpeqb	k1, zmm1, zmm0
	kortestq	k1, k1
	jne			.end
	add			rax, 64
	jmp			.loop_align

.end:
	kmovq		rdx, k1
	tzcnt		rdx, rdx
	sub			rax, rdi
	add			rax, rdx
	ret

