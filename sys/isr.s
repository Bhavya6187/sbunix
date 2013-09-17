.global _isr_000

_isr_000:
  cli
  pushq 0x00      # Push a dummy error code (if ISR0 doesn't push it's own error code)
  pushq 0x01      # Push the interrupt number 0
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
  
 # movq %rax, %ds         # Lower 16-bits of eax = ds.
 # pushq %rax            # save the data segment descriptor
 # movq %rax, 0x10       # load the kernel data segment descriptor

  movq  %rsp,%rdi
  addq  $72, %rdi

 # movq %ds, %rax
 # movq %es, %rax
 # movq %fs, %rax
 # movq %gs, %rax
 # movq %ss, %rax
  
  call isr_handler_0
  
 # popq %rax        # reload the original data segment descriptor
 # movq %ds, %rax
 # movq %es, %rax
 # movq %fs, %rax
 # movq %gs, %rax
 # movq %ss, %rax
  
  pushq %r15
  pushq %r14
  pushq %r13
  pushq %r12
  pushq %r11
  pushq %r10
  pushq %r9
  pushq %r8
  popq %rsp
  popq %rbp
  popq %rdi
  popq %rsi
  popq %rdx
  popq %rcx
  popq %rbx
  popq %rax        # Pops the general purpose registers GPR's R8-R15 also might come
  
 # addq %rsp, 8     # Cleans up the pushed error code and pushed ISR number
  
  iretq           

