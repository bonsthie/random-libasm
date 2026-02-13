- s1 and s2 are align the word is beautiful
- there is a cross page in s1 or s2 in the next 4 VEC_SIZE
    - now if that s1 or s2
        - if that s1 set r8 to 0 and eax to the remaing byte
        - if s2 swap rdi rsi, set r8 to -1 and eax to the remaing byte
    - loop of unaligement untile the last one
    - that the vec of page cross and eax is a the offset of the last byte of the page so do this vec 
- rdi is align so we loop untile rsi will cross a pge

# the aligment of rdi

## init

PAGE_SIZE = 4096
VEC_SIZE=32

s1 = 14090 <- near page end
s2 = 22222
```asm
strcmp:
 vpxor %xmm15, %xmm15, %xmm15
 movl %edi, %eax
 orl %esi, %eax
 sall $20, %eax

 cmpl $((4096 -(32 * 4)) << 20), %eax
 ja .Lpage_cross

.Lno_page_cross:
```
c code
```c
xmm15 = 0;
eax = (s1 | s2) << 20
if (eax > (PAGE_SIZE -(VEC_SIZE * 4))) << 20)
    goto .Lpage_cross
```

c equivalent to
```c
xmm15 = 0;
eax = (s1 | s2) % PAGE_SIZE
if (eax > (PAGE_SIZE - (VEC_SIZE * 4)))
    goto .Lpage_cross
```
so this code look if any of s1 or s2 cross a page in the next 4 VEC_SIZE

## page_cross
```asm
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
 xorl %r8d, %r8d ; 

 subl $(32 * 3), %eax
 jg .Lless_1x_vec_till_page
```
c equivalent
```c
    if (eax % VEC_SIZE == 0)
        goto .Lno_page_cross;
    
    eax = (u32)s1 % PAGE_SIZE;
    ecx = (u32)s2 % PAGE_SIZE;

    rdx = 0; // is set to 0 if s1 and -1 if s2 to invese the res of the end cmp

    if (ecx < eax)
        goto .Lpage_cross_s2

    // if there is no page cross in the next 4 VEC_SIZE
    // eq to if ((eax <= PAGE_SIZE - 4*VEC_SIZE)
    eax -= PAGE_SIZE - VEC_SIZE * 4;
    if (eax <= 0)
        goto .Lno_page_cross

    r8d = 0;
    
    // if the cross page is in the next VEC
    eax -= VEC_SIZE * 3
    if (eax > 0)
        goto .Lless_1x_vec_till_page

    // now eax range between -(VEC_SIZE * 4) and 0 so -128 and 0

```

## Lpage_cross_s2
```asm

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
```
- swap chain
- set r8d to -1 so that they can swap the result
- goto page_cross_loop

## page_cross_loop

```asm
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
```
c equivalent
```c
    ymm0 = vec load rdi + rdx;
    ymm1 = cmp vec(rsi + rdx) == ymm0;
    ymm2 = cmp ymm15 == ymm0; // zero mask
    ymm1 &= ymm2
    ecx = mask of ymm1

    ecx++
    if (ecx has overflow the mask was full so the cmp is valid)
        goto .Lcheck_ret_vec_page_cross

    edx += 32;
    eax += 32;

    edx -= eax;


```
example of the loop increment
```c
loop:
// do cmp not aligned for both

edx = 0;
eax = -95;

edx += 32 = 32;
eax += 32 = -64;
if (eax < 0) goto loop

edx += 32 = 64;
eax += 32 = -31;
if (eax < 0) goto loop
    
edx += 32 = 96;
eax += 32 = 1;
if (eax < 0) goto loop

edx -= eax = 96 - 1 = 95

```
edx ends up being the offset where you can place the last full 32-byte vector load that still stays inside the page.
```asm
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
 VZEROUPPER_RETURN ; ret
```

- get the offset of the mask in ecx
- add it to the offset in edx
- get both byte at offset s1 + edx and s2 + edx 
- sub them for the ret value
- xor it to r8b to inverse if this is from s2 (if ecx = 32 xor -1 ecx = 33)
- sub r8b to eax to get the -32
- ret

note :
They increment ecx after each vector compare for two reasons:
All-equal case:
If the mask is 0xFFFFFFFF, inc ecx overflows to 0, which cleanly signals “no mismatch, keep looping”.

Mismatch/zero case:
The mask has the form (1 << i) – 1.
After inc ecx, it becomes 1 << i, so tzcnt directly returns the mismatch index i—no extra +1 needed.

# rdi is align
now eather s1 or s2 is align in rdi.
the goal is to continue vec comparaison and only making sure rsi is not crossing any page

```asm
.Lprepare_loop:
.Lprepare_loop_no_len:
 subq %rdi, %rsi
 andq $-(32 * 4), %rdi
 addq %rdi, %rsi

.Lprepare_loop_aligned:
 movl $-(32 * 4), %eax
 subl %esi, %eax
 andl $(4096 - 1), %eax

.Lloop:
```





note :
p2align 4,, 10
.p2align LOG , FILL , MAX
align 4 log 2
with 0 (default)
off by ≤ 10 bytes -> padding inserted
