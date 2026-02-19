
.text
.intel_syntax noprefix

.globl  __strlen_avx2
.hidden __strlen_avx2
.type   __strlen_avx2, @function

.equ    PAGE_SIZE, 4096
.equ    VEC_SIZE,  32

/* size_t __strlen_avx2(const char *str)
 * str -> rdi
 */
__strlen_avx2:
        mov     rax, rdi                    /* rax = endptr */
        xor     rdx, rdx                    /* rdx = mask offset */
        vpxor   ymm0, ymm0, ymm0            /* zeros mask */

        /* is there a missing alignment */
        mov     esi, edi
        and     rsi, (VEC_SIZE - 1)
        jz      .loop_align

        /* check page alignment */
        mov     ecx, edi
        and     ecx, (PAGE_SIZE - 1)
        cmp     ecx, (PAGE_SIZE - VEC_SIZE) /* in last 31 bytes? */
        ja      .near_page_end

        /* misalignment offset in rcx = VEC_SIZE - (str & (VEC_SIZE-1)) */
        mov     rcx, VEC_SIZE
        sub     rcx, rsi

        /* first cmp unaligned */
        vmovdqu ymm1, YMMWORD PTR [rax]     /* unaligned load */
        vpcmpeqb ymm1, ymm1, ymm0
        vpmovmskb edx, ymm1                 /* generate mask */
        test    edx, edx
        jne     .end
        add     rax, rcx

.loop_align:
        vpcmpeqb ymm1, ymm0, YMMWORD PTR [rax]  /* aligned load compare */
        vpmovmskb edx, ymm1
        test    edx, edx
        jne     .end
        add     rax, VEC_SIZE
        jmp     .loop_align

.end:
        tzcnt   edx, edx
        sub     rax, rdi
        add     rax, rdx
        ret

.near_page_end:
        /* See NASM comments: avoid crossing into next page with a wide load. */
        or      rax, (VEC_SIZE - 1)
        vpcmpeqb ymm1, ymm0, YMMWORD PTR [rax - (VEC_SIZE - 1)]
        vpmovmskb edx, ymm1
        shrx    edx, edx, esi               /* drop bytes before string start */
        test    edx, edx
        je      .near_page_no_zero

        tzcnt   eax, edx
        ret

.near_page_no_zero:
        inc     rax
        jmp     .loop_align

        .size   __strlen_avx2, .-__strlen_avx2
