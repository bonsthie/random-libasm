# 1 "strcmp-avx2.S"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 412 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "strcmp-avx2.S" 2
# 19 "strcmp-avx2.S"
# 1 "../../x86/isa-level.h" 1
# 20 "strcmp-avx2.S" 2








# 1 "./strcmp-naming.h" 1
# 29 "strcmp-avx2.S" 2

# 1 "../../x86/sysdep.h" 1
# 22 "../../x86/sysdep.h"
# 1 "../../../sysdeps/generic/sysdep.h" 1
# 97 "../../../sysdeps/generic/sysdep.h"
# 1 "../../../sysdeps/generic/dwarf2.h" 1
# 98 "../../../sysdeps/generic/sysdep.h" 2
# 23 "../../x86/sysdep.h" 2
# 31 "strcmp-avx2.S" 2
# 188 "strcmp-avx2.S"
 .section .text.avx, "ax", @progbits
 .align 16
 .type strcmp, @function
 .globl strcmp
# 204 "strcmp-avx2.S"
 .p2align 4
strcmp:
 .cfi_startproc
 _CET_ENDBR
 CALL_MCOUNT
# 250 "strcmp-avx2.S"
 vpxor %xmm15, %xmm15, %xmm15
# 275 "strcmp-avx2.S"
 movl %edi, %eax
 orl %esi, %eax
 sall $20, %eax

 cmpl $((4096 -(32 * 4)) << 20), %eax
 ja .Lpage_cross

.Lno_page_cross:

 vmovdqu (%rdi), %ymm0
 vpcmpeqb (%rsi), %ymm0, %ymm1
 vpcmpeqb %ymm0, %ymm15, %ymm2
 vpandn %ymm1, %ymm2, %ymm1
 vpmovmskb %ymm1, %ecx
 incl %ecx
 jz .Lmore_3x_vec

 .p2align 4,, 4
.Lreturn_vec_0:
 tzcntl %ecx, %ecx
 movzbl (%rdi, %rcx), %eax
 movzbl (%rsi, %rcx), %ecx
 subl %ecx, %eax

.Lret0:
.Lreturn_vzeroupper:
 ZERO_UPPER_VEC_REGISTERS_RETURN
# 370 "strcmp-avx2.S"
 .p2align 4,, 10
.Lreturn_vec_1:
 tzcntl %ecx, %ecx
# 389 "strcmp-avx2.S"
 movzbl 32(%rdi, %rcx), %eax
 movzbl 32(%rsi, %rcx), %ecx


 subl %ecx, %eax

.Lret2:
 VZEROUPPER_RETURN

 .p2align 4,, 10





.Lreturn_vec_2:

 tzcntl %ecx, %ecx
# 422 "strcmp-avx2.S"
 movzbl (32 * 2)(%rdi, %rcx), %eax
 movzbl (32 * 2)(%rsi, %rcx), %ecx


 subl %ecx, %eax

.Lret3:
 VZEROUPPER_RETURN


 .p2align 4,, 10
.Lreturn_vec_3:
 tzcntl %ecx, %ecx
# 444 "strcmp-avx2.S"
 movzbl (32 * 3)(%rdi, %rcx), %eax
 movzbl (32 * 3)(%rsi, %rcx), %ecx


 subl %ecx, %eax

.Lret4:
 VZEROUPPER_RETURN


 .p2align 4,, 10
.Lmore_3x_vec:

 vmovdqu 32(%rdi), %ymm0
 vpcmpeqb 32(%rsi), %ymm0, %ymm1
 vpcmpeqb %ymm0, %ymm15, %ymm2
 vpandn %ymm1, %ymm2, %ymm1
 vpmovmskb %ymm1, %ecx
 incl %ecx
 jnz .Lreturn_vec_1






 vmovdqu (32 * 2)(%rdi), %ymm0
 vpcmpeqb (32 * 2)(%rsi), %ymm0, %ymm1
 vpcmpeqb %ymm0, %ymm15, %ymm2
 vpandn %ymm1, %ymm2, %ymm1
 vpmovmskb %ymm1, %ecx
 incl %ecx
 jnz .Lreturn_vec_2

 vmovdqu (32 * 3)(%rdi), %ymm0
 vpcmpeqb (32 * 3)(%rsi), %ymm0, %ymm1
 vpcmpeqb %ymm0, %ymm15, %ymm2
 vpandn %ymm1, %ymm2, %ymm1
 vpmovmskb %ymm1, %ecx
 incl %ecx
 jnz .Lreturn_vec_3
# 497 "strcmp-avx2.S"
 xorl %r8d, %r8d




.Lprepare_loop:
.Lprepare_loop_no_len:
 subq %rdi, %rsi
 andq $-(32 * 4), %rdi
 addq %rdi, %rsi

.Lprepare_loop_aligned:
 movl $-(32 * 4), %eax
 subl %esi, %eax
 andl $(4096 - 1), %eax


 .p2align 4
.Lloop:
 subq $-(32 * 4), %rdi
 subq $-(32 * 4), %rsi


 addl $-(32 * 4), %eax
 jnb .Lpage_cross_during_loop


.Lloop_skip_page_cross_check:
 vmovdqa (32 * 0)(%rdi), %ymm0
 vmovdqa (32 * 1)(%rdi), %ymm2
 vmovdqa (32 * 2)(%rdi), %ymm4
 vmovdqa (32 * 3)(%rdi), %ymm6


 vpcmpeqb (32 * 0)(%rsi), %ymm0, %ymm1
 vpcmpeqb (32 * 1)(%rsi), %ymm2, %ymm3
 vpcmpeqb (32 * 2)(%rsi), %ymm4, %ymm5
 vpcmpeqb (32 * 3)(%rsi), %ymm6, %ymm7

 vpand %ymm0, %ymm1, %ymm1
 vpand %ymm2, %ymm3, %ymm3
 vpand %ymm4, %ymm5, %ymm5
 vpand %ymm6, %ymm7, %ymm7

 vpminub %ymm1, %ymm3, %ymm3
 vpminub %ymm5, %ymm7, %ymm7
 vpminub %ymm3, %ymm7, %ymm7


 vpcmpeqb %ymm7, %ymm15, %ymm7
 vpmovmskb %ymm7, %edx
 testl %edx, %edx
 jz .Lloop


 vpcmpeqb %ymm1, %ymm15, %ymm1
 vpmovmskb %ymm1, %ecx
 testl %ecx, %ecx
 jnz .Lreturn_vec_0_end


 vpcmpeqb %ymm3, %ymm15, %ymm3
 vpmovmskb %ymm3, %ecx
 testl %ecx, %ecx
 jnz .Lreturn_vec_1_end

.Lreturn_vec_2_3_end:





 vpcmpeqb %ymm5, %ymm15, %ymm5
 vpmovmskb %ymm5, %ecx
 testl %ecx, %ecx
 jnz .Lreturn_vec_2_end





 tzcntl %edx, %edx
# 624 "strcmp-avx2.S"
 movzbl (32 * 2 - (-32))(%rdi, %rdx), %eax
 movzbl (32 * 2 - (-32))(%rsi, %rdx), %ecx


 subl %ecx, %eax
 xorl %r8d, %eax
 subl %r8d, %eax

.Lret5:
 VZEROUPPER_RETURN
# 646 "strcmp-avx2.S"
 .p2align 4,, 10




.Lreturn_vec_0_end:

 tzcntl %ecx, %ecx
# 669 "strcmp-avx2.S"
 movzbl (%rdi, %rcx), %eax
 movzbl (%rsi, %rcx), %ecx


 subl %ecx, %eax
 xorl %r8d, %eax
 subl %r8d, %eax

.Lret6:
 VZEROUPPER_RETURN


 .p2align 4,, 10
.Lreturn_vec_1_end:
 tzcntl %ecx, %ecx
# 693 "strcmp-avx2.S"
 movzbl 32(%rdi, %rcx), %eax
 movzbl 32(%rsi, %rcx), %ecx


 subl %ecx, %eax
 xorl %r8d, %eax
 subl %r8d, %eax

.Lret7:
 VZEROUPPER_RETURN


 .p2align 4,, 10
.Lreturn_vec_2_end:
 tzcntl %ecx, %ecx
# 721 "strcmp-avx2.S"
 movzbl (32 * 2)(%rdi, %rcx), %eax
 movzbl (32 * 2)(%rsi, %rcx), %ecx


 subl %ecx, %eax
 xorl %r8d, %eax
 subl %r8d, %eax

.Lret11:
 VZEROUPPER_RETURN





 .p2align 4,, 10
.Lpage_cross_during_loop:




 cmpl $-(32 * 4), %eax


 je .Lloop_skip_page_cross_check


 cmpl $-(32 * 3), %eax
 jle .Lless_1x_vec_till_page_cross

 vmovdqa (%rdi), %ymm0
 vpcmpeqb (%rsi), %ymm0, %ymm1
 vpcmpeqb %ymm0, %ymm15, %ymm2
 vpandn %ymm1, %ymm2, %ymm1
 vpmovmskb %ymm1, %ecx
 incl %ecx
 jnz .Lreturn_vec_0_end


 cmpl $-(32 * 2), %eax
 jg .Lmore_2x_vec_till_page_cross

 .p2align 4,, 4
.Lless_1x_vec_till_page_cross:
 subl $-(32 * 4), %eax







 vmovdqu -32(%rdi, %rax), %ymm0
 vpcmpeqb -32(%rsi, %rax), %ymm0, %ymm1
 vpcmpeqb %ymm0, %ymm15, %ymm2
 vpandn %ymm1, %ymm2, %ymm1
 vpmovmskb %ymm1, %ecx



 movl $-1, %r10d
 shlxl %esi, %r10d, %r10d
 notl %ecx





 movl %eax, %edx
 addl $(4096 - 32 * 4), %eax

 andl %r10d, %ecx
 jz .Lloop_skip_page_cross_check

 .p2align 4,, 3
.Lreturn_page_cross_end:
 tzcntl %ecx, %ecx





 addl %edx, %ecx
# 814 "strcmp-avx2.S"
 movzbl (-32)(%rdi, %rcx), %eax
 movzbl (-32)(%rsi, %rcx), %ecx


 subl %ecx, %eax
 xorl %r8d, %eax
 subl %r8d, %eax

.Lret8:
 VZEROUPPER_RETURN
# 838 "strcmp-avx2.S"
 .p2align 4,, 10
.Lmore_2x_vec_till_page_cross:



 vmovdqu 32(%rdi), %ymm0
 vpcmpeqb 32(%rsi), %ymm0, %ymm1
 vpcmpeqb %ymm0, %ymm15, %ymm2
 vpandn %ymm1, %ymm2, %ymm1
 vpmovmskb %ymm1, %ecx
 incl %ecx
 jnz .Lreturn_vec_1_end






 subl $-(32 * 4), %eax


 vmovdqu -(32 * 2)(%rdi, %rax), %ymm0
 vpcmpeqb -(32 * 2)(%rsi, %rax), %ymm0, %ymm1
 vpcmpeqb %ymm0, %ymm15, %ymm2
 vpandn %ymm1, %ymm2, %ymm1
 vpmovmskb %ymm1, %ecx
 incl %ecx
 jnz .Lreturn_vec_page_cross_0

 vmovdqu -(32 * 1)(%rdi, %rax), %ymm0
 vpcmpeqb -(32 * 1)(%rsi, %rax), %ymm0, %ymm1
 vpcmpeqb %ymm0, %ymm15, %ymm2
 vpandn %ymm1, %ymm2, %ymm1
 vpmovmskb %ymm1, %ecx
 incl %ecx
 jnz .Lreturn_vec_page_cross_1
# 883 "strcmp-avx2.S"
 vmovdqa (32 * 2)(%rdi), %ymm4
 vmovdqa (32 * 3)(%rdi), %ymm6

 vpcmpeqb (32 * 2)(%rsi), %ymm4, %ymm5
 vpcmpeqb (32 * 3)(%rsi), %ymm6, %ymm7
 vpand %ymm4, %ymm5, %ymm5
 vpand %ymm6, %ymm7, %ymm7
 vpminub %ymm5, %ymm7, %ymm7
 vpcmpeqb %ymm7, %ymm15, %ymm7
 vpmovmskb %ymm7, %edx
 testl %edx, %edx
 jnz .Lreturn_vec_2_3_end





 subq $-(32 * 4), %rdi
 subq $-(32 * 4), %rsi
 addl $(4096 - 32 * 8), %eax







 jmp .Lloop_skip_page_cross_check



 .p2align 4,, 10
.Lreturn_vec_page_cross_0:
 addl $-32, %eax
.Lreturn_vec_page_cross_1:
 tzcntl %ecx, %ecx





 addl %eax, %ecx
# 936 "strcmp-avx2.S"
 movzbl (-32)(%rdi, %rcx), %eax
 movzbl (-32)(%rsi, %rcx), %ecx


 subl %ecx, %eax
 xorl %r8d, %eax
 subl %r8d, %eax

.Lret9:
 VZEROUPPER_RETURN


 .p2align 4,, 10
.Lpage_cross:

 testl $((32 - 1) << 20), %eax
 jz .Lno_page_cross


 movl %edi, %eax
 movl %esi, %ecx
 andl $(4096 - 1), %eax
 andl $(4096 - 1), %ecx

 xorl %edx, %edx


 cmpl %eax, %ecx
 jg .Lpage_cross_s2



 subl $(4096 - 32 * 4), %eax
 jbe .Lno_page_cross
# 981 "strcmp-avx2.S"
 xorl %r8d, %r8d



 subl $(32 * 3), %eax
 jg .Lless_1x_vec_till_page




 .p2align 4,, 10
.Lpage_cross_loop:

 vmovdqu (%rdi, %rdx), %ymm0
 vpcmpeqb (%rsi, %rdx), %ymm0, %ymm1
 vpcmpeqb %ymm0, %ymm15, %ymm2
 vpandn %ymm1, %ymm2, %ymm1
 vpmovmskb %ymm1, %ecx
 incl %ecx
 jnz .Lcheck_ret_vec_page_cross

 addl $32, %edx
 addl $32, %eax
 jl .Lpage_cross_loop

 subl %eax, %edx
 vmovdqu (%rdi, %rdx), %ymm0
 vpcmpeqb (%rsi, %rdx), %ymm0, %ymm1
 vpcmpeqb %ymm0, %ymm15, %ymm2
 vpandn %ymm1, %ymm2, %ymm1
 vpmovmskb %ymm1, %ecx
 incl %ecx
 jz .Lprepare_loop_no_len

 .p2align 4,, 4
.Lret_vec_page_cross:

.Lcheck_ret_vec_page_cross:

 tzcntl %ecx, %ecx
 addl %edx, %ecx
.Lret_vec_page_cross_cont:
# 1048 "strcmp-avx2.S"
 movzbl (%rdi, %rcx), %eax
 movzbl (%rsi, %rcx), %ecx


 subl %ecx, %eax
 xorl %r8d, %eax
 subl %r8d, %eax

.Lret12:
 VZEROUPPER_RETURN
# 1074 "strcmp-avx2.S"
 .p2align 4,, 4
.Lpage_cross_s2:

 subl $(4096 - 32 * 4), %ecx
 jbe .Lno_page_cross


 movl %ecx, %eax
 movq %rdi, %rcx
 movq %rsi, %rdi
 movq %rcx, %rsi





 movl $-1, %r8d

 xorl %edx, %edx


 subl $(32 * 3), %eax
 jle .Lpage_cross_loop

 .p2align 4,, 6
.Lless_1x_vec_till_page:

 cmpl $16, %eax
 ja .Lless_16_till_page

 vmovdqu (%rdi), %xmm0
 vpcmpeqb (%rsi), %xmm0, %xmm1
 vpcmpeqb %xmm0, %xmm15, %xmm2
 vpandn %xmm1, %xmm2, %xmm1
 vpmovmskb %ymm1, %ecx
 incw %cx
 jnz .Lcheck_ret_vec_page_cross
 movl $16, %edx






 subl %eax, %edx
 jz .Lprepare_loop


 vmovdqu (%rdi, %rdx), %xmm0
 vpcmpeqb (%rsi, %rdx), %xmm0, %xmm1
 vpcmpeqb %xmm0, %xmm15, %xmm2
 vpandn %xmm1, %xmm2, %xmm1
 vpmovmskb %ymm1, %ecx
 incw %cx
 jnz .Lcheck_ret_vec_page_cross
# 1139 "strcmp-avx2.S"
 leaq (16 - 32 * 4)(%rdi, %rdx), %rdi
 leaq (16 - 32 * 4)(%rsi, %rdx), %rsi

 jmp .Lprepare_loop_aligned
# 1152 "strcmp-avx2.S"
 .p2align 4,, 10
.Lless_16_till_page:

 cmpl $24, %eax
 ja .Lless_8_till_page

 vmovq (%rdi), %xmm0
 vmovq (%rsi), %xmm1
 vpcmpeqb %xmm0, %xmm15, %xmm2
 vpcmpeqb %xmm1, %xmm0, %xmm1
 vpandn %xmm1, %xmm2, %xmm1
 vpmovmskb %ymm1, %ecx
 incb %cl
 jnz .Lcheck_ret_vec_page_cross






 movl $24, %edx

 subl %eax, %edx



 vmovq (%rdi, %rdx), %xmm0
 vmovq (%rsi, %rdx), %xmm1
 vpcmpeqb %xmm0, %xmm15, %xmm2
 vpcmpeqb %xmm1, %xmm0, %xmm1
 vpandn %xmm1, %xmm2, %xmm1
 vpmovmskb %ymm1, %ecx
 incb %cl
 jnz .Lcheck_ret_vec_page_cross
# 1196 "strcmp-avx2.S"
 leaq (8 - 32 * 4)(%rdi, %rdx), %rdi
 leaq (8 - 32 * 4)(%rsi, %rdx), %rsi

 jmp .Lprepare_loop_aligned


 .p2align 4,, 10
.Lless_8_till_page:
# 1231 "strcmp-avx2.S"
 cmpl $28, %eax
 ja .Lless_4_till_page

 vmovd (%rdi), %xmm0
 vmovd (%rsi), %xmm1
 vpcmpeqb %xmm0, %xmm15, %xmm2
 vpcmpeqb %xmm1, %xmm0, %xmm1
 vpandn %xmm1, %xmm2, %xmm1
 vpmovmskb %ymm1, %ecx
 subl $0xf, %ecx
 jnz .Lcheck_ret_vec_page_cross





 movl $28, %edx

 subl %eax, %edx



 vmovd (%rdi, %rdx), %xmm0
 vmovd (%rsi, %rdx), %xmm1
 vpcmpeqb %xmm0, %xmm15, %xmm2
 vpcmpeqb %xmm1, %xmm0, %xmm1
 vpandn %xmm1, %xmm2, %xmm1
 vpmovmskb %ymm1, %ecx
 subl $0xf, %ecx
 jnz .Lcheck_ret_vec_page_cross
# 1271 "strcmp-avx2.S"
 leaq (4 - 32 * 4)(%rdi, %rdx), %rdi
 leaq (4 - 32 * 4)(%rsi, %rdx), %rsi

 jmp .Lprepare_loop_aligned
# 1283 "strcmp-avx2.S"
 .p2align 4,, 10
.Lless_4_till_page:
 subq %rdi, %rsi

.Lless_4_loop:
 movzbl (%rdi), %eax
 movzbl (%rsi, %rdi), %ecx


 subl %ecx, %eax
 jnz .Lret_less_4_loop
 testl %ecx, %ecx
 jz .Lret_zero_4_loop




 incq %rdi

 testl $31, %edi
 jnz .Lless_4_loop
 leaq -(32 * 4)(%rdi, %rsi), %rsi
 addq $-(32 * 4), %rdi



 jmp .Lprepare_loop_aligned

.Lret_zero_4_loop:
 xorl %eax, %eax
 ret
.Lret_less_4_loop:
 xorl %r8d, %eax
 subl %r8d, %eax
 ret

 .cfi_endproc
 .size strcmp, .-strcmp
