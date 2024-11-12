	.file	"ModelFunctionNotInlined.user.c"
	.text
	.p2align 4
	.globl	add
	.type	add, @function
add:
.LFB13:
	.cfi_startproc
	endbr64
	movslq	%esi, %rax
	addq	%rdi, %rax
	ret
	.cfi_endproc
.LFE13:
	.size	add, .-add
	.p2align 4
	.globl	sub
	.type	sub, @function
sub:
.LFB14:
	.cfi_startproc
	endbr64
	negl	%esi
	jmp	add
	.cfi_endproc
.LFE14:
	.size	sub, .-sub
	.p2align 4
	.globl	mul
	.type	mul, @function
mul:
.LFB15:
	.cfi_startproc
	endbr64
	movq	%rsi, %r8
	testq	%rsi, %rsi
	jle	.L7
	movl	%edi, %ecx
	xorl	%edx, %edx
	xorl	%edi, %edi
	.p2align 4,,10
	.p2align 3
.L6:
	movl	%ecx, %esi
	addq	$1, %rdx
	call	add
	movq	%rax, %rdi
	cmpq	%rdx, %r8
	jne	.L6
	movq	%rdi, %rax
	ret
	.p2align 4,,10
	.p2align 3
.L7:
	xorl	%edi, %edi
	movq	%rdi, %rax
	ret
	.cfi_endproc
.LFE15:
	.size	mul, .-mul
	.p2align 4
	.globl	div_int
	.type	div_int, @function
div_int:
.LFB16:
	.cfi_startproc
	endbr64
	movq	%rdi, %rdx
	movq	%rsi, %r8
	xorl	%ecx, %ecx
	cmpq	%rsi, %rdi
	jl	.L9
	.p2align 4,,10
	.p2align 3
.L11:
	movq	%rdx, %rdi
	movq	%r8, %rsi
	call	sub
	movq	%rcx, %rdi
	movl	$1, %esi
	movq	%rax, %rdx
	call	add
	movq	%rax, %rcx
	cmpq	%rdx, %r8
	jle	.L11
.L9:
	movq	%rcx, %rax
	ret
	.cfi_endproc
.LFE16:
	.size	div_int, .-div_int
	.p2align 4
	.globl	performComplexComputation
	.type	performComplexComputation, @function
performComplexComputation:
.LFB17:
	.cfi_startproc
	endbr64
	movq	%rdi, %r9
	testq	%rdi, %rdi
	jle	.L17
	xorl	%r10d, %r10d
	xorl	%edi, %edi
	.p2align 4,,10
	.p2align 3
.L16:
	movl	$10, %esi
	addq	$1, %r10
	call	add
	movl	$5, %esi
	movq	%rax, %rdi
	call	sub
	movl	$3, %esi
	movq	%rax, %rdi
	call	mul
	movl	$2, %esi
	movq	%rax, %rdi
	call	div_int
	movq	%rax, %rdi
	cmpq	%r10, %r9
	jne	.L16
	movq	%rdi, %rax
	ret
	.p2align 4,,10
	.p2align 3
.L17:
	xorl	%edi, %edi
	movq	%rdi, %rax
	ret
	.cfi_endproc
.LFE17:
	.size	performComplexComputation, .-performComplexComputation
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
	call	clock@PLT
	movl	$100, %edi
	movq	%rax, %rbp
	call	performComplexComputation
	movq	%rax, %r12
	call	clock@PLT
	movq	%r12, %rdx
	movl	$2, %edi
	leaq	.LC0(%rip), %rsi
	movq	%rax, %rbx
	xorl	%eax, %eax
	call	__printf_chk@PLT
	subq	%rbp, %rbx
	pxor	%xmm0, %xmm0
	leaq	.LC2(%rip), %rsi
	movl	$2, %edi
	cvtsi2sdq	%rbx, %xmm0
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
