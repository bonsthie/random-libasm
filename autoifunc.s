	.intel_syntax noprefix
	.file	"autoifunc.c"
	.text
	.globl	ft_strlen.default.2             # -- Begin function ft_strlen.default.2
	.p2align	4
	.type	ft_strlen.default.2,@function
ft_strlen.default.2:                    # @ft_strlen.default.2
	.cfi_startproc
# %bb.0:
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset rbp, -16
	mov	rbp, rsp
	.cfi_def_cfa_register rbp
	mov	qword ptr [rbp - 8], rdi
.LBB0_1:                                # =>This Inner Loop Header: Depth=1
	mov	rax, qword ptr [rbp - 16]
	cmp	byte ptr [rax], 0
	je	.LBB0_3
# %bb.2:                                #   in Loop: Header=BB0_1 Depth=1
	mov	rax, qword ptr [rbp - 16]
	add	rax, 1
	mov	qword ptr [rbp - 16], rax
	jmp	.LBB0_1
.LBB0_3:
	mov	rax, qword ptr [rbp - 16]
	mov	rcx, qword ptr [rbp - 8]
	sub	rax, rcx
                                        # kill: def $eax killed $eax killed $rax
	pop	rbp
	.cfi_def_cfa rsp, 8
	ret
.Lfunc_end0:
	.size	ft_strlen.default.2, .Lfunc_end0-ft_strlen.default.2
	.cfi_endproc
                                        # -- End function
	.globl	ft_strlen.sse.0                 # -- Begin function ft_strlen.sse.0
	.p2align	4
	.type	ft_strlen.sse.0,@function
ft_strlen.sse.0:                        # @ft_strlen.sse.0
	.cfi_startproc
# %bb.0:
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset rbp, -16
	mov	rbp, rsp
	.cfi_def_cfa_register rbp
	mov	qword ptr [rbp - 8], rdi
.LBB1_1:                                # =>This Inner Loop Header: Depth=1
	mov	rax, qword ptr [rbp - 16]
	cmp	byte ptr [rax], 0
	je	.LBB1_3
# %bb.2:                                #   in Loop: Header=BB1_1 Depth=1
	mov	rax, qword ptr [rbp - 16]
	add	rax, 1
	mov	qword ptr [rbp - 16], rax
	jmp	.LBB1_1
.LBB1_3:
	mov	rax, qword ptr [rbp - 16]
	mov	rcx, qword ptr [rbp - 8]
	sub	rax, rcx
                                        # kill: def $eax killed $eax killed $rax
	pop	rbp
	.cfi_def_cfa rsp, 8
	ret
.Lfunc_end1:
	.size	ft_strlen.sse.0, .Lfunc_end1-ft_strlen.sse.0
	.cfi_endproc
                                        # -- End function
	.globl	ft_strlen.avx2.1                # -- Begin function ft_strlen.avx2.1
	.p2align	4
	.type	ft_strlen.avx2.1,@function
ft_strlen.avx2.1:                       # @ft_strlen.avx2.1
	.cfi_startproc
# %bb.0:
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset rbp, -16
	mov	rbp, rsp
	.cfi_def_cfa_register rbp
	mov	qword ptr [rbp - 8], rdi
.LBB2_1:                                # =>This Inner Loop Header: Depth=1
	mov	rax, qword ptr [rbp - 16]
	cmp	byte ptr [rax], 0
	je	.LBB2_3
# %bb.2:                                #   in Loop: Header=BB2_1 Depth=1
	mov	rax, qword ptr [rbp - 16]
	add	rax, 1
	mov	qword ptr [rbp - 16], rax
	jmp	.LBB2_1
.LBB2_3:
	mov	rax, qword ptr [rbp - 16]
	mov	rcx, qword ptr [rbp - 8]
	sub	rax, rcx
                                        # kill: def $eax killed $eax killed $rax
	pop	rbp
	.cfi_def_cfa rsp, 8
	ret
.Lfunc_end2:
	.size	ft_strlen.avx2.1, .Lfunc_end2-ft_strlen.avx2.1
	.cfi_endproc
                                        # -- End function
	.section	.text.ft_strlen.resolver,"axG",@progbits,ft_strlen.resolver,comdat
	.weak	ft_strlen.resolver              # -- Begin function ft_strlen.resolver
	.p2align	4
	.type	ft_strlen.resolver,@function
ft_strlen.resolver:                     # @ft_strlen.resolver
	.cfi_startproc
# %bb.0:
	push	rax
	.cfi_def_cfa_offset 16
	call	__cpu_indicator_init
	mov	eax, dword ptr [rip + __cpu_model+12]
	and	eax, 1024
	cmp	eax, 1024
	sete	al
	and	al, 1
	test	al, 1
	jne	.LBB3_1
	jmp	.LBB3_2
.LBB3_1:
	lea	rax, [rip + ft_strlen.avx2.1]
	pop	rcx
	.cfi_def_cfa_offset 8
	ret
.LBB3_2:
	.cfi_def_cfa_offset 16
	mov	eax, dword ptr [rip + __cpu_model+12]
	and	eax, 8
	cmp	eax, 8
	sete	al
	and	al, 1
	test	al, 1
	jne	.LBB3_3
	jmp	.LBB3_4
.LBB3_3:
	lea	rax, [rip + ft_strlen.sse.0]
	pop	rcx
	.cfi_def_cfa_offset 8
	ret
.LBB3_4:
	.cfi_def_cfa_offset 16
	lea	rax, [rip + ft_strlen.default.2]
	pop	rcx
	.cfi_def_cfa_offset 8
	ret
.Lfunc_end3:
	.size	ft_strlen.resolver, .Lfunc_end3-ft_strlen.resolver
	.cfi_endproc
                                        # -- End function
	.weak	ft_strlen.ifunc
	.type	ft_strlen.ifunc,@function
ft_strlen.ifunc = ft_strlen
	.weak	ft_strlen
	.type	ft_strlen,@gnu_indirect_function
ft_strlen = ft_strlen.resolver
	.ident	"clang version 21.1.5"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym ft_strlen.default.2
	.addrsig_sym ft_strlen.sse.0
	.addrsig_sym ft_strlen.avx2.1
	.addrsig_sym ft_strlen.resolver
	.addrsig_sym __cpu_indicator_init
	.addrsig_sym __cpu_model
	.addrsig_sym ft_strlen
