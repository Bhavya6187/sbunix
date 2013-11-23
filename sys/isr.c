#include <sys/idt.h>
#include <sys/isr.h>
#include <stdio.h>

void isr_handler_0(registers_t regs)
{
     printf("Error: Divide by zero.. !! Grrr.... \n");
     // Pushed by pushq i.e. all general purpose registers
     //printf(" r15 %x, r14 %x, r13 %x, r12 %x, r11 %x, r10 %x, r9 %x, r8 %x,\n rsp %x, rbp %x, rdi %x, rsi %x, rdx %x, rcx %x, rbx %x, rax %x \n",     regs.r15, regs.r14, regs.r13, regs.r12, regs.r11, regs.r10, regs.r9, regs.r8, regs.rsp, regs.rbp, regs.rdi, regs.rsi, regs.rdx, regs.rcx, regs.rbx, regs.rax);
     printf("Interrupt Number = %d %d\n", regs.interrupt_number, regs.err_code);
     //printf("rds = %x\n", regs.ds);
     //printf(" rip %x, cs %x, eflags %x, usersp %x\n", regs.rip, regs.cs, regs.eflags, regs.usersp);
     printf("Success !!\n");
     while(1);
} 

void isr_handler_13(registers_t regs)
{
     printf("Exception: General Protection Fault !! Grrr.... \n");
     // Pushed by pushq i.e. all general purpose registers
     // printf(" r15 %x, r14 %x, r13 %x, r12 %x, r11 %x, r10 %x, r9 %x, r8 %x,\n rsp %x, rbp %x, rdi %x, rsi %x, rdx %x, rcx %x, rbx %x, rax %x \n",     regs.r15, regs.r14, regs.r13, regs.r12, regs.r11, regs.r10, regs.r9, regs.r8, regs.rsp, regs.rbp, regs.rdi, regs.rsi, regs.rdx, regs.rcx, regs.rbx, regs.rax);
    uint64_t cr2;
     //Read cr3 here -
  	__asm volatile(
		"movq %%cr2, %0"
		:"=g"(cr2)
		:
	  :"memory"
	  );
  
     printf("Interrupt Number = %d errCode=%d cr=%p\n", regs.interrupt_number, regs.err_code, cr2);
     //printf("rds = %x\n", regs.ds);
     //printf(" rip %x, cs %x, eflags %x, usersp %x\n", regs.rip, regs.cs, regs.eflags, regs.usersp);
     printf("Success !!\n");
     while(1);
}

void isr_handler_14(registers_t regs)
{
     printf("Exception: Page Fault !! Lol .. !! Grrr.... \n");
     // Pushed by pushq i.e. all general purpose registers
     // printf(" r15 %x, r14 %x, r13 %x, r12 %x, r11 %x, r10 %x, r9 %x, r8 %x,\n rsp %x, rbp %x, rdi %x, rsi %x, rdx %x, rcx %x, rbx %x, rax %x \n",     regs.r15, regs.r14, regs.r13, regs.r12, regs.r11, regs.r10, regs.r9, regs.r8, regs.rsp, regs.rbp, regs.rdi, regs.rsi, regs.rdx, regs.rcx, regs.rbx, regs.rax);
    uint64_t cr2;
     //Read cr3 here -
  	__asm volatile(
		"movq %%cr2, %0"
		:"=g"(cr2)
		:
	  :"memory"
	  );
  
     printf("Interrupt Number = %d %d %p\n", regs.interrupt_number, regs.err_code, cr2);
     //printf("rds = %x\n", regs.ds);
     //printf(" rip %x, cs %x, eflags %x, usersp %x\n", regs.rip, regs.cs, regs.eflags, regs.usersp);
     printf("Success !!\n");
     while(1);
}

void isr_handler_80(registers_t regs)
{
    clrscr();
     printf("Interrupt 80 for system calls .... \n");
    /*uint64_t cr2;
     //Read cr2 here -
  	__asm volatile(
		"movq %%cr2, %0"
		:"=g"(cr2)
		:
	  :"memory"
	  );
  
     printf("Interrupt Number = %d %d %p\n", regs.interrupt_number, regs.err_code, cr2);
     printf("Success !!\n");
     while(1);*/
}

/*
  movq $31, %rdi
  pushq %rdi      # Push the interrupt number 0
  popq %rdi        # Pops the general purpose registers GPR's R8-R15 also might come
     uint64_t ds;                  // Data segment selector
     uint64_t rax, rbx, rcx, rdx, rdi, rsi, rbp, rsp; // Pushed by pushq i.e. all general purpose registers
     unsigned char interrupt_number, err_code;    // Interrupt number and error code (if applicable)
     uint64_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
*/

