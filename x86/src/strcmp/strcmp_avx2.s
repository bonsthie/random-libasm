section .text
global __strcmp_avx2:hidden

%define PAGE_SIZE 4096
%define VEC_SIZE 32


; goal
; - s1 and s2 are align the word is beautiful
; - there is a cross page in s1 or s2 in the next 4 VEC_SIZE
;     - now if that s1 or s2
;         - if that s1 set r8 to 0 and eax to the remaing byte
;         - if s2 swap rdi rsi, set r8 to -1 and eax to the remaing byte
;     - loop of unaligement untile the last one
;     - that the vec of page cross and eax is a the offset of the last byte of the page so do this vec 
; - rdi is align so we loop untile rsi will cross a pge

__strcmp_avx2:
	vpxor	xmm0, xmm0, xmm0
	xor		r8b, r8b				; is s1 have en swap for page aligment


.check_for_page_cross_in_both_string:
	; is there a page cross in the next 4 page in 
	mov		eax, edi
	or		eax, esi
	sall	eax, 20
	cmp		eax, (PAGE_SIZE - (VEC_SIZE * 4)) << 20
	ja		.page_cross_in_next_4


	; is there a missing aligment
	mov			rdx, edi
	and			rdx, VEC_SIZE - 1
	jnz			.align_rdi_string_no_page_cross

.loop_align_4x:
	vmovdqa		ymm1, [rdi]
	vpcmpeqb	ymm2, ymm1, [rsi]
	vpcmpeqb	ymm3, ymm1, ymm0
	vpand		ymm2, ymm2, ymm3
	vpmovmsk	ecx, ymm1

	add			rdi, VEC_SIZE
	add			rsi,VEC_SIZEdx
	inc			ecx
	jnz			.end

.loop_align_3x:
	vmovdqa		ymm1, [rdi + (VEC_SIZE * 0)]
	vmovdqa		ymm2, [rdi + (VEC_SIZE * 1)]
	vmovdqa		ymm3, [rdi + (VEC_SIZE * 2)]

	vpcmpeq		ymm4, ymm1, [rsi + (VEC_SIZE * 0)]
	vpcmpeq		ymm5, ymm2, [rsi + (VEC_SIZE * 1)]
	vpcmpeq		ymm6, ymm3, [rsi + (VEC_SIZE * 2)]

	; looking for '\0'
	vpand		ymm4, ymm1, ymm4
	vpand		ymm5, ymm2, ymm5
	vpand		ymm6, ymm3, ymm6

	; fuse all the vector by taking the lowest value of each byte
	vpminub		ymm7, ymm4, ymm5
	vpminub		ymm7, ymm7, ymm6

	vpcmpeq		ymm7, ymm7, ymm0
	vpmovmsk	ecx, ymm7
	inc			ecx
	jz			.loop_align_4x
.ret:
	...

.align_rdi_string_no_page_cross:
	vmovdqu		ymm1, [rdi]
	vpcmpeqb	ymm2, ymm1, [rsi]
	vpcmpeqb	ymm3, ymm1, ymm0
	vpand		ymm2, ymm2, ymm3
	vpmovmsk	ecx, ymm1

	add			rdi, rdx
	add			rsi, rdx
	inc			ecx
	jz			.loop_align_3x
	jmp			.end





.page_cross_in_next_4:
