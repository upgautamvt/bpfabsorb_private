	.text
	.file	"ModelFunctionNotInlined.user.c"
	.globl	add                             # -- Begin function add
	.p2align	4, 0x90
	.type	add,@function
add:                                    # @add
	.cfi_startproc
# %bb.0:
	movslq	%esi, %rax
	addq	%rdi, %rax
	retq
.Lfunc_end0:
	.size	add, .Lfunc_end0-add
	.cfi_endproc
                                        # -- End function
	.globl	sub                             # -- Begin function sub
	.p2align	4, 0x90
	.type	sub,@function
sub:                                    # @sub
	.cfi_startproc
# %bb.0:
	negl	%esi
                                        # kill: def $esi killed $esi killed $rsi
	jmp	add                             # TAILCALL
.Lfunc_end1:
	.size	sub, .Lfunc_end1-sub
	.cfi_endproc
                                        # -- End function
	.globl	mul                             # -- Begin function mul
	.p2align	4, 0x90
	.type	mul,@function
mul:                                    # @mul
	.cfi_startproc
# %bb.0:
	testq	%rsi, %rsi
	jle	.LBB2_1
# %bb.3:
	pushq	%r14
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	pushq	%rax
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -24
	.cfi_offset %r14, -16
	movq	%rsi, %rbx
	movq	%rdi, %r14
	xorl	%eax, %eax
	.p2align	4, 0x90
.LBB2_4:                                # =>This Inner Loop Header: Depth=1
	movq	%rax, %rdi
	movl	%r14d, %esi
	callq	add
	decq	%rbx
	jne	.LBB2_4
# %bb.5:
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	.cfi_restore %rbx
	.cfi_restore %r14
	retq
.LBB2_1:
	xorl	%eax, %eax
	retq
.Lfunc_end2:
	.size	mul, .Lfunc_end2-mul
	.cfi_endproc
                                        # -- End function
	.globl	div_int                         # -- Begin function div_int
	.p2align	4, 0x90
	.type	div_int,@function
div_int:                                # @div_int
	.cfi_startproc
# %bb.0:
	pushq	%r15
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	.cfi_offset %r15, -16
	cmpq	%rsi, %rdi
	jge	.LBB3_2
# %bb.1:
	xorl	%r14d, %r14d
	jmp	.LBB3_4
.LBB3_2:
	movq	%rsi, %rbx
	movq	%rdi, %r15
	xorl	%r14d, %r14d
	.p2align	4, 0x90
.LBB3_3:                                # =>This Inner Loop Header: Depth=1
	movq	%r15, %rdi
	movq	%rbx, %rsi
	callq	sub
	movq	%rax, %r15
	movq	%r14, %rdi
	movl	$1, %esi
	callq	add
	movq	%rax, %r14
	cmpq	%rbx, %r15
	jge	.LBB3_3
.LBB3_4:
	movq	%r14, %rax
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end3:
	.size	div_int, .Lfunc_end3-div_int
	.cfi_endproc
                                        # -- End function
	.globl	performComplexComputation       # -- Begin function performComplexComputation
	.p2align	4, 0x90
	.type	performComplexComputation,@function
performComplexComputation:              # @performComplexComputation
	.cfi_startproc
# %bb.0:
	testq	%rdi, %rdi
	jle	.LBB4_1
# %bb.3:
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	xorl	%eax, %eax
	.p2align	4, 0x90
.LBB4_4:                                # =>This Inner Loop Header: Depth=1
	movq	%rax, %rdi
	movl	$10, %esi
	callq	add
	movl	$5, %esi
	movq	%rax, %rdi
	callq	sub
	movl	$3, %esi
	movq	%rax, %rdi
	callq	mul
	movl	$2, %esi
	movq	%rax, %rdi
	callq	div_int
	decq	%rbx
	jne	.LBB4_4
# %bb.5:
	popq	%rbx
	.cfi_def_cfa_offset 8
	.cfi_restore %rbx
	retq
.LBB4_1:
	xorl	%eax, %eax
	retq
.Lfunc_end4:
	.size	performComplexComputation, .Lfunc_end4-performComplexComputation
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function main
.LCPI5_0:
	.quad	0x412e848000000000              # double 1.0E+6
	.text
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%r15
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	.cfi_offset %r15, -16
	callq	clock@PLT
	movq	%rax, %rbx
	movl	$100, %edi
	callq	performComplexComputation
	movq	%rax, %r14
	callq	clock@PLT
	movq	%rax, %r15
	leaq	.L.str(%rip), %rdi
	movq	%r14, %rsi
	xorl	%eax, %eax
	callq	printf@PLT
	subq	%rbx, %r15
	cvtsi2sd	%r15, %xmm0
	divsd	.LCPI5_0(%rip), %xmm0
	leaq	.L.str.1(%rip), %rdi
	movb	$1, %al
	callq	printf@PLT
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end5:
	.size	main, .Lfunc_end5-main
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"Result: %ld\n"
	.size	.L.str, 13

	.type	.L.str.1,@object                # @.str.1
.L.str.1:
	.asciz	"Execution time: %lf seconds\n"
	.size	.L.str.1, 29

	.ident	"Ubuntu clang version 18.1.3 (1ubuntu1)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
