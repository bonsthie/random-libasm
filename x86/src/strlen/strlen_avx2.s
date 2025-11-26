
section .text
global __strlen_avx2:hidden

%define PAGE_SIZE 4096
%define VEC_SIZE 32

; size_t __strlen_base(const char *str)
; str -> rdi
__strlen_avx2:
	mov			rax, rdi					; rax = endptr
	xor			rdx, rdx					; rdx = mask offset
	vpxor		ymm0, ymm0, ymm0			; zeros mask

	; is there a missing aligment
	mov			esi, edi
	and			rsi, VEC_SIZE - 1
	jz			.loop_align

	; check page aligment
	mov			ecx, edi
	and			ecx, PAGE_SIZE - 1
	cmp			ecx, PAGE_SIZE - VEC_SIZE   ; in last 31 bytes?
	ja			.near_page_end  

	; miss aligment offset in rcx
	mov			rcx, VEC_SIZE
	sub			rcx, rsi

	; first cmp unalign
	vmovdqu		ymm1, [rax]					; unalign load
	vpcmpeqb	ymm1, ymm1, ymm0			; compare
	vpmovmskb	edx, ymm1					; generate mask
	cmp			edx, 0
	jne			.end
	add			rax, rcx

.loop_align:
	vpcmpeqb	ymm1, ymm0, [rax]			; align load compare
	vpmovmskb	edx, ymm1					; generate mask
	cmp			edx, 0
	jne			.end
	add			rax, VEC_SIZE
	jmp			.loop_align

.end:
	tzcnt		edx, edx
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
	vpcmpeqb	ymm1, ymm0, [rax - (VEC_SIZE - 1)]
	vpmovmskb	edx, ymm1
	shrx		edx, edx, esi
	cmp			edx, 0
	je			.near_page_no_zero

	tzcnt		eax, edx
	ret

.near_page_no_zero:
    inc       rax
    jmp       .loop_align
