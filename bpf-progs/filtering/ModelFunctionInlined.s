	.file	"ModelFunctionInlined.user.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Result: %ld\n"
.LC2:
	.string	"Execution time: %lf seconds\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB18:
	.cfi_startproc
	endbr64
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	xorl	%ebx, %ebx
	call	clock@PLT
	movl	$100, %ecx
	movq	%rax, %r12
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L9:
	subq	$2, %rax
	shrq	%rax
	leaq	1(%rax), %rbx
	subq	$1, %rcx
	je	.L8
.L3:
	leaq	5(%rbx), %rdx
	movslq	%edx, %rdx
	leaq	(%rdx,%rdx,2), %rax
	cmpq	$1, %rax
	jg	.L9
	xorl	%ebx, %ebx
	subq	$1, %rcx
	jne	.L3
.L8:
	call	clock@PLT
	movq	%rbx, %rdx
	movl	$2, %edi
	leaq	.LC0(%rip), %rsi
	movq	%rax, %rbp
	xorl	%eax, %eax
	call	__printf_chk@PLT
	subq	%r12, %rbp
	pxor	%xmm0, %xmm0
	leaq	.LC2(%rip), %rsi
	movl	$2, %edi
	cvtsi2sdq	%rbp, %xmm0
	movl	$1, %eax
	divsd	.LC1(%rip), %xmm0
	call	__printf_chk@PLT
	popq	%rbx
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE18:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC1:
	.long	0
	.long	1093567616
	.ident	"GCC: (Ubuntu 13.2.0-23ubuntu4) 13.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
