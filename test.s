	.intel_syntax noprefix
	.file	"test.c"
	.text
	.globl	ft_strlen_0                     # -- Begin function ft_strlen_0
	.p2align	4
	.type	ft_strlen_0,@function
ft_strlen_0:                            # @ft_strlen_0
	.cfi_startproc
# %bb.0:
	jmp	strlen@PLT                      # TAILCALL
.Lfunc_end0:
	.size	ft_strlen_0, .Lfunc_end0-ft_strlen_0
	.cfi_endproc
                                        # -- End function
	.globl	ft_strlen_1                     # -- Begin function ft_strlen_1
	.p2align	4
	.type	ft_strlen_1,@function
ft_strlen_1:                            # @ft_strlen_1
	.cfi_startproc
# %bb.0:
	push	rax
	.cfi_def_cfa_offset 16
	call	strlen@PLT
	inc	rax
	pop	rcx
	.cfi_def_cfa_offset 8
	ret
.Lfunc_end1:
	.size	ft_strlen_1, .Lfunc_end1-ft_strlen_1
	.cfi_endproc
                                        # -- End function
	.globl	ft_strlen_campte                # -- Begin function ft_strlen_campte
	.p2align	4
	.type	ft_strlen_campte,@function
ft_strlen_campte:                       # @ft_strlen_campte
	.cfi_startproc
# %bb.0:
	mov	eax, dword ptr [rip + i]
	cmp	eax, 1
	lea	rcx, [rip + ft_strlen_0]
	lea	rax, [rip + ft_strlen_1]
	cmove	rax, rcx
	ret
.Lfunc_end2:
	.size	ft_strlen_campte, .Lfunc_end2-ft_strlen_campte
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	push	rbp
	.cfi_def_cfa_offset 16
	push	rbx
	.cfi_def_cfa_offset 24
	push	rax
	.cfi_def_cfa_offset 32
	.cfi_offset rbx, -24
	.cfi_offset rbp, -16
	mov	rbx, rsi
	mov	ebp, edi
	lea	rdi, [rip + .Lstr.1]
	call	puts@PLT
	cmp	ebp, 2
	jne	.LBB3_2
# %bb.1:
	mov	rdi, qword ptr [rbx + 8]
	call	ft_strlen
                                        # kill: def $eax killed $eax killed $rax
	add	rsp, 8
	.cfi_def_cfa_offset 24
	pop	rbx
	.cfi_def_cfa_offset 16
	pop	rbp
	.cfi_def_cfa_offset 8
	ret
.LBB3_2:
	.cfi_def_cfa_offset 32
	lea	rdi, [rip + .Lstr.1]
	call	puts@PLT
	xor	eax, eax
                                        # kill: def $eax killed $eax killed $rax
	add	rsp, 8
	.cfi_def_cfa_offset 24
	pop	rbx
	.cfi_def_cfa_offset 16
	pop	rbp
	.cfi_def_cfa_offset 8
	ret
.Lfunc_end3:
	.size	main, .Lfunc_end3-main
	.cfi_endproc
                                        # -- End function
	.type	i,@object                       # @i
	.data
	.globl	i
	.p2align	2, 0x0
i:
	.long	1                               # 0x1
	.size	i, 4

	.type	.Lstr.1,@object                 # @str.1
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr.1:
	.asciz	"caca"
	.size	.Lstr.1, 5

	.globl	ft_strlen
	.type	ft_strlen,@gnu_indirect_function
ft_strlen = ft_strlen_campte
	.ident	"clang version 21.1.5"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym ft_strlen_0
	.addrsig_sym ft_strlen_1
	.addrsig_sym ft_strlen_campte
	.addrsig_sym i
	.addrsig_sym ft_strlen
