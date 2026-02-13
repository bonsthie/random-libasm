
section .text
global __strlen_avx512:hidden

%define PAGE_SIZE 4096
%define VEC_SIZE 64

; size_t __strlen_base(const char *str)
; str -> rdi
__strlen_avx512:
	mov			rax, rdi			; rax = endptr
	vpxorq		zmm0, zmm0, zmm0	; zmm0 = zeros mask

	; is there a missing aligment
	mov			esi, edi
	and			rsi, VEC_SIZE - 1
	jz			.loop_align

	; check page aligment
	mov			ecx, edi
	and			ecx, PAGE_SIZE - 1
	cmp			ecx, PAGE_SIZE - VEC_SIZE   ; in last 63 bytes?
	ja			.near_page_end  


	; miss aligment offset in rcx
	mov			rcx, VEC_SIZE
	sub			rcx, rsi

	; first cmp unalign
	vpcmpeqb	k1, zmm0, [rax]
	kortestq	k1, k1
	jne			.end
	add			rax, rcx

.loop_align:
	vmovdqa64	zmm1, [rax] 
	vpcmpeqb	k1, zmm0, zmm1
	kortestq	k1, k1
	jne			.end
	add			rax, VEC_SIZE
	jmp			.loop_align

.end:
	kmovq		rdx, k1
	tzcnt		rdx, rdx
	sub			rax, rdi
	add			rax, rdx
	ret


.near_page_end:
	; ex : PAGE_SIZE 4096 VEC_SIZE 32
	; [0 - 4095][4096-8192]
	;		  | <- string start at 4080 and end at 4112 so there is an overlap
	; mov rax, 4080
	; or rax, VEC_SIZE - 1 -> rax  = 4095
	; vector compare of rax - 32
	; make mask
	; shift unwanted bit [4064-4079]
	; cmp mask, 0
	; je .loop_align
	; ret mask
	or			rax, (VEC_SIZE - 1)
	vmovdqa64	zmm1, [rax - (VEC_SIZE - 1)] 
	vpcmpeqb	k1, zmm0, zmm1
	kmovq		rdx, k1
	shrx		rdx, rdx, rsi
	test		rdx, rdx
	je			.near_page_end_no_zero 

	tzcnt		eax, edx
	ret

.near_page_end_no_zero:
    inc       rax
    jmp       .loop_align
