	.file	"muchaCPU.c"
	.text
	.section	.rodata
	.align 8
.LC4:
	.string	"Iteraciones en 1 segundo: %llu\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movl	%edi, -68(%rbp)
	movq	%rsi, -80(%rbp)
	movsd	.LC0(%rip), %xmm0
	movsd	%xmm0, -40(%rbp)
	movsd	.LC1(%rip), %xmm0
	movsd	%xmm0, -32(%rbp)
	movq	$0, -56(%rbp)
	call	clock@PLT
	movq	%rax, -48(%rbp)
.L4:
	movsd	-40(%rbp), %xmm0
	addsd	-32(%rbp), %xmm0
	divsd	-40(%rbp), %xmm0
	movsd	-32(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -24(%rbp)
	addq	$1, -56(%rbp)
	call	clock@PLT
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	subq	-48(%rbp), %rax
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rax, %xmm0
	movsd	.LC2(%rip), %xmm1
	divsd	%xmm1, %xmm0
	movsd	%xmm0, -8(%rbp)
	movsd	-8(%rbp), %xmm0
	movsd	.LC3(%rip), %xmm1
	comisd	%xmm1, %xmm0
	jb	.L2
	movq	-56(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC4(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	$0, -56(%rbp)
	call	clock@PLT
	movq	%rax, -48(%rbp)
.L2:
	jmp	.L4
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC0:
	.long	1729588965
	.long	1073033210
	.align 8
.LC1:
	.long	382849949
	.long	1073864572
	.align 8
.LC2:
	.long	0
	.long	1093567616
	.align 8
.LC3:
	.long	0
	.long	1072693248
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
