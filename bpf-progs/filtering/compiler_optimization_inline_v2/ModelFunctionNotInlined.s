	.file	"ModelFunctionNotInlined.user.c"
	.text
	.p2align 4
	.globl	getValue
	.type	getValue, @function
getValue:
.LFB24:
	.cfi_startproc
	endbr64
	movl	$10, %eax
	ret
	.cfi_endproc
.LFE24:
	.size	getValue, .-getValue
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC2:
	.string	"Inside loop."
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC3:
	.string	"This gets not eliminated in non-inlined."
	.section	.rodata.str1.1
.LC5:
	.string	"Sum of matrix elements: %f\n"
	.section	.rodata.str1.8
	.align 8
.LC7:
	.string	"Execution time (not inlined): %lf seconds\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB25:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$1, %edi
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r13
	pushq	%r12
	leaq	-80(%rbp), %rsi
	pushq	%rbx
	.cfi_offset 13, -24
	.cfi_offset 12, -32
	.cfi_offset 3, -40
	movl	$1000000000, %ebx
	subq	$56, %rsp
	movq	%fs:40, %rax
	movq	%rax, -40(%rbp)
	xorl	%eax, %eax
	call	clock_gettime@PLT
	xorl	%eax, %eax
	call	getValue
	movl	%eax, %r12d
	jmp	.L7
	.p2align 4,,10
	.p2align 3
.L4:
	subq	$1, %rbx
	je	.L6
.L7:
	cmpl	$100, %r12d
	jle	.L4
	leaq	.LC2(%rip), %r13
	.p2align 4,,10
	.p2align 3
.L5:
	movq	%r13, %rdi
	call	puts@PLT
	subq	$1, %rbx
	jne	.L5
.L6:
	cmpl	$100, %r12d
	jg	.L24
.L8:
	leaq	-64(%rbp), %rsi
	movl	$1, %edi
	call	clock_gettime@PLT
	movq	-56(%rbp), %rax
	pxor	%xmm0, %xmm0
	subq	-72(%rbp), %rax
	cvtsi2sdq	%rax, %xmm0
	pxor	%xmm1, %xmm1
	movq	-64(%rbp), %rax
	subq	-80(%rbp), %rax
	cvtsi2sdq	%rax, %xmm1
	movl	$2, %edi
	movl	$1, %eax
	divsd	.LC6(%rip), %xmm0
	leaq	.LC7(%rip), %rsi
	addsd	%xmm1, %xmm0
	call	__printf_chk@PLT
	movq	-40(%rbp), %rax
	subq	%fs:40, %rax
	jne	.L25
	leaq	-24(%rbp), %rsp
	xorl	%eax, %eax
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L24:
	.cfi_restore_state
	leaq	.LC3(%rip), %rdi
	movq	%rsp, %rbx
	call	puts@PLT
	leaq	-77824(%rsp), %rax
.L9:
	cmpq	%rax, %rsp
	je	.L10
	subq	$4096, %rsp
	orq	$0, 4088(%rsp)
	jmp	.L9
.L10:
	movl	$2176, %eax
	subq	$2176, %rsp
	orq	$0, 2168(%rsp)
	movdqa	.LC4(%rip), %xmm5
	movdqa	.LC0(%rip), %xmm6
	xorl	%esi, %esi
	movq	%rsp, %r9
	leaq	800(%r9), %rdx
	movq	%r9, %rdi
	movq	%r9, %r8
	movq	%rdx, %rcx
.L12:
	movd	%esi, %xmm7
	movq	%r8, %rax
	movdqa	%xmm6, %xmm2
	pshufd	$0, %xmm7, %xmm3
	movdqa	%xmm3, %xmm4
	psrlq	$32, %xmm4
	.p2align 4,,10
	.p2align 3
.L13:
	movdqa	%xmm2, %xmm1
	addq	$32, %rax
	paddd	%xmm5, %xmm2
	movdqa	%xmm1, %xmm0
	psrlq	$32, %xmm1
	pmuludq	%xmm3, %xmm0
	pmuludq	%xmm4, %xmm1
	pshufd	$8, %xmm0, %xmm0
	pshufd	$8, %xmm1, %xmm1
	punpckldq	%xmm1, %xmm0
	cvtdq2pd	%xmm0, %xmm1
	pshufd	$238, %xmm0, %xmm0
	movups	%xmm1, -32(%rax)
	cvtdq2pd	%xmm0, %xmm0
	movups	%xmm0, -16(%rax)
	cmpq	%rcx, %rax
	jne	.L13
	addl	$1, %esi
	addq	$800, %r8
	leaq	800(%rax), %rcx
	cmpl	$100, %esi
	jne	.L12
	leaq	80800(%r9), %rcx
	pxor	%xmm0, %xmm0
.L14:
	movq	%rdi, %rax
	.p2align 4,,10
	.p2align 3
.L15:
	addsd	(%rax), %xmm0
	addq	$16, %rax
	addsd	-8(%rax), %xmm0
	cmpq	%rdx, %rax
	jne	.L15
	leaq	800(%rax), %rdx
	addq	$800, %rdi
	cmpq	%rcx, %rdx
	jne	.L14
	leaq	.LC5(%rip), %rsi
	movl	$2, %edi
	movl	$1, %eax
	call	__printf_chk@PLT
	movq	%rbx, %rsp
	jmp	.L8
.L25:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE25:
	.size	main, .-main
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC0:
	.long	0
	.long	1
	.long	2
	.long	3
	.align 16
.LC4:
	.long	4
	.long	4
	.long	4
	.long	4
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC6:
	.long	0
	.long	1104006501
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
