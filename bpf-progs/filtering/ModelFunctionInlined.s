	.text
	.file	"ModelFunctionInlined.user.c"
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function main
.LCPI0_0:
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
	movl	$100, %r15d
	xorl	%r14d, %r14d
	callq	clock@PLT
	movq	%rax, %rbx
	jmp	.LBB0_1
	.p2align	4, 0x90
.LBB0_3:                                #   in Loop: Header=BB0_1 Depth=1
	leaq	(%rax,%rax,2), %r14
	cmpq	$3, %r14
	movl	$3, %eax
	cmovlq	%r14, %rax
	subq	%rax, %r14
	incq	%r14
	shrq	%r14
	incq	%r14
	decq	%r15
	je	.LBB0_5
.LBB0_1:                                # =>This Inner Loop Header: Depth=1
	addl	$5, %r14d
	movslq	%r14d, %rax
	testq	%rax, %rax
	jg	.LBB0_3
# %bb.2:                                #   in Loop: Header=BB0_1 Depth=1
	xorl	%r14d, %r14d
	decq	%r15
	jne	.LBB0_1
.LBB0_5:
	callq	clock@PLT
	movq	%rax, %r15
	leaq	.L.str(%rip), %rdi
	movq	%r14, %rsi
	xorl	%eax, %eax
	callq	printf@PLT
	subq	%rbx, %r15
	cvtsi2sd	%r15, %xmm0
	divsd	.LCPI0_0(%rip), %xmm0
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
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
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
