.global _ptcr3
.global _ptcr0
_ptcr3:

	movq %rdi,%rax 
	movq %rax,%cr3             # write the cr3 register
	retq

_ptcr0:

	movq %rdi,%rax 
	movq %rax,%cr0             # write the cr0 register
	retq      
