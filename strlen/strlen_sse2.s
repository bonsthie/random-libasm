
section .text
global __strlen_sse2:hidden

%define PAGE_SIZE 4096
%define VEC_SIZE 16

; size_t __strlen_sse2(const char *str)
; str -> rdi
__strlen_sse2:
	mov			rax, rdi					; rax = endptr
	xor			rdx, rdx					; rdx = mask offset
	pxor		xmm0, xmm0					; zeros mask

	; is there a missing aligment
	mov			esi, edi
	and			esi, VEC_SIZE - 1
	jz			.loop_align

	; check page aligment
	mov			ecx, edi
	and			ecx, PAGE_SIZE - 1
	cmp			ecx, PAGE_SIZE - VEC_SIZE   ; in last 15 bytes?
	ja			.near_page_end  


	; miss aligment offset in rcx
	mov			rcx, VEC_SIZE
	sub			rcx, rsi

	; first cmp unalign
	movdqu		xmm1, [rax]					; unalign load
	pcmpeqb		xmm1, xmm0					; compare
	pmovmskb	edx, xmm1					; generate mask
	cmp			edx, 0
	jne			.end
	add			rax, rcx

.loop_align:
	movdqa		xmm1, [rax]					; align load
	pcmpeqb		xmm1, xmm0					; align load compare
	pmovmskb	edx, xmm1					; generate mask
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
	; ex : PAGE_SIZE 4096 VEC_SIZE 16
	; [0 - 4095][4096-8192]
	;		  | <- string start at 4090 and end at 4112 so there is an overlap
	; mov rax, 4080
	; or rax, VEC_SIZE - 1 -> rax  = 4095
	; vector compare of rax - 16
	; make mask
	; shift unwanted bit [4079-4089]
	; cmp mask, 0
	; je .loop_align
	; ret mask

	or			rax, VEC_SIZE - 1
	movdqa		xmm1, [rax - (VEC_SIZE - 1)]
	pcmpeqb		xmm1, xmm0
	pmovmskb	edx, xmm1
	shrx		edx, edx, esi
	cmp			edx, 0
	je			.near_page_no_zero

	tzcnt		eax, edx
	ret

.near_page_no_zero:
    inc       rax
    jmp       .loop_align
