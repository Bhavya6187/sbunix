.global _isr_000

_isr_000:
  cli
  pushq byte 0         # Push a dummy error code (if ISR0 doesn't push it's own error code)
  pushq byte 0         # Push the interrupt number (0)
  jmp isr_common_stub  # Go to our common handler. 
  sti
  retq

isr_common_stub:
  pushq %rax
  pushq %rbx
  pushq %rcx
  pushq %rdx
  pushq %rdi
  pushq %rsi
  pushq %rbp
  pushq %rsp           # Pushes the general purpose registers GPR's R8-R15 also might come
  call isr_handler_0
  popq %rsp
  popq %rbp
  popq %rsi
  popq %rdi
  popq %rdx
  popq %rcx
  popq %rbx
  popq %rax            # Pops the general purpose registers GPR's R8-R15 also might come
  sti
  iretq           

