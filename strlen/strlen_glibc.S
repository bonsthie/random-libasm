# 1 "strlen-avx2.S"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 412 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "strlen-avx2.S" 2
# 19 "strlen-avx2.S"
# 1 "../../x86/isa-level.h" 1
# 20 "strlen-avx2.S" 2




# 1 "../../x86/sysdep.h" 1
# 22 "../../x86/sysdep.h"
# 1 "../../../sysdeps/generic/sysdep.h" 1
# 97 "../../../sysdeps/generic/sysdep.h"
# 1 "../../../sysdeps/generic/dwarf2.h" 1
# 98 "../../../sysdeps/generic/sysdep.h" 2
# 23 "../../x86/sysdep.h" 2
# 25 "strlen-avx2.S" 2
# 52 "strlen-avx2.S"
 .section .text.avx,"ax",@progbits
ENTRY_P2ALIGN (__strlen_avx2, 4)
# 66 "strlen-avx2.S"
 movl %edi, %eax
 movq %rdi, %rdx
 vpxor %xmm0, %xmm0, %xmm0


 andl $(4096 - 1), %eax

 cmpl $(4096 - 32), %eax
 ja .Lcross_page_boundary

 vpcmpeqb (%rdi), %ymm0, %ymm1
 vpmovmskb %ymm1, %eax

 testl %eax, %eax
 jz .Laligned_more
 tzcntl %eax, %eax
 ret

# 117 "strlen-avx2.S"
 .p2align 4
.Lfirst_vec_x1:
 tzcntl %eax, %eax
# 132 "strlen-avx2.S"
 subl %edx, %edi
 incl %edi
 addl %edi, %eax
 ret

 .p2align 4
.Lfirst_vec_x2:
 tzcntl %eax, %eax
# 157 "strlen-avx2.S"
 subl %edx, %edi
 addl $(32 + 1), %edi
 addl %edi, %eax
 ret


 .p2align 4
.Lfirst_vec_x3:
 tzcntl %eax, %eax
# 182 "strlen-avx2.S"
 subl %edx, %edi
 addl $(32 * 2 + 1), %edi
 addl %edi, %eax
 ret

 .p2align 4
.Lfirst_vec_x4:
 tzcntl %eax, %eax
# 207 "strlen-avx2.S"
 subl %edx, %edi
 addl $(32 * 3 + 1), %edi
 addl %edi, %eax
 ret

 .p2align 5
.Laligned_more:

 orq $(32 - 1), %rdi
.Lcross_page_continue:
# 237 "strlen-avx2.S"
 vpcmpeqb 1(%rdi), %ymm0, %ymm1

 vpmovmskb %ymm1, %eax
 testl %eax, %eax
 jnz .Lfirst_vec_x1

 vpcmpeqb (32 + 1)(%rdi), %ymm0, %ymm1
 vpmovmskb %ymm1, %eax
 testl %eax, %eax
 jnz .Lfirst_vec_x2

 vpcmpeqb (32 * 2 + 1)(%rdi), %ymm0, %ymm1
 vpmovmskb %ymm1, %eax
 testl %eax, %eax
 jnz .Lfirst_vec_x3

 vpcmpeqb (32 * 3 + 1)(%rdi), %ymm0, %ymm1
 vpmovmskb %ymm1, %eax
 testl %eax, %eax
 jnz .Lfirst_vec_x4
# 278 "strlen-avx2.S"
 incq %rdi
 orq $(32 * 4 - 1), %rdi


 .p2align 4
.Lloop_4x_vec:
# 293 "strlen-avx2.S"
 vmovdqa 1(%rdi), %ymm1
 vpminub (32 + 1)(%rdi), %ymm1, %ymm2
 vmovdqa (32 * 2 + 1)(%rdi), %ymm3
 vpminub (32 * 3 + 1)(%rdi), %ymm3, %ymm4

 vpminub %ymm2, %ymm4, %ymm5
 vpcmpeqb %ymm5, %ymm0, %ymm5
 vpmovmskb %ymm5, %ecx

 subq $-(32 * 4), %rdi
 testl %ecx, %ecx
 jz .Lloop_4x_vec


 vpcmpeqb %ymm1, %ymm0, %ymm1
 vpmovmskb %ymm1, %eax
 subq %rdx, %rdi
 testl %eax, %eax
 jnz .Llast_vec_return_x0

 vpcmpeqb %ymm2, %ymm0, %ymm2
 vpmovmskb %ymm2, %eax
 testl %eax, %eax
 jnz .Llast_vec_return_x1


 vpcmpeqb %ymm3, %ymm0, %ymm3
 vpmovmskb %ymm3, %eax


 salq $32, %rcx
 orq %rcx, %rax
 tzcntq %rax, %rax
 subq $(32 * 2 - 1), %rdi
 addq %rdi, %rax
 ret

# 380 "strlen-avx2.S"
 .p2align 4
.Llast_vec_return_x0:
 tzcntl %eax, %eax
 subq $(32 * 4 - 1), %rdi
 addq %rdi, %rax
 ret

 .p2align 4
.Llast_vec_return_x1:
 tzcntl %eax, %eax
 subq $(32 * 3 - 1), %rdi
 addq %rdi, %rax
 ret
# 509 "strlen-avx2.S"
 .p2align 4
.Lcross_page_boundary:

 orq $(32 - 1), %rdi
 vpcmpeqb -(32 - 1)(%rdi), %ymm0, %ymm1
 vpmovmskb %ymm1, %eax


 sarxl %edx, %eax, %eax
# 532 "strlen-avx2.S"
 testl %eax, %eax
 jz .Lcross_page_continue
 tzcntl %eax, %eax

.Lreturn_vzeroupper:
 ZERO_UPPER_VEC_REGISTERS_RETURN
# 556 "strlen-avx2.S"
.cfi_endproc; .size __strlen_avx2,.-__strlen_avx2;
