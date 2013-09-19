.global _isr_000

_isr_000:
  cli
  movq $31, %rdi
  pushq %rdi      # Push the interrupt number 0
  jmp isr_common_stub  # Go to our common handler. 
  sti
  iretq

isr_common_stub:
  pushq %rax
  pushq %rbx
  pushq %rcx
  pushq %rdx
  pushq %rsi
  pushq %rdi
  pushq %rbp
  pushq %rsp           # Pushes the general purpose registers GPR's R8-R15 also might come
  pushq %r8
  pushq %r9
  pushq %r10
  pushq %r11
  pushq %r12
  pushq %r13
  pushq %r14
  pushq %r15

  #trying to restore the code segment, instruction pointer, flags register, stack segment and stack pointer                         
  
  call isr_handler_0
  
  popq %r15
  popq %r14
  popq %r13
  popq %r12
  popq %r11
  popq %r10
  popq %r9
  popq %r8
  popq %rsp
  popq %rbp
  popq %rdi
  popq %rsi
  popq %rdx
  popq %rcx
  popq %rbx
  popq %rax        # Pops the general purpose registers GPR's R8-R15 also might come
  popq %rdi        # Pops the general purpose registers GPR's R8-R15 also might come
  
  iretq           

