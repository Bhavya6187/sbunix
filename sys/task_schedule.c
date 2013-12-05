#include<sys/task_management.h>
#include<sys/v_mem_manager.h>
#include<sys/gdt.h>
#include<stdio.h>

void schedule_process()
{
	PCB  *next_process=NULL;
  // Add current process to runableQ
  
  ///-----------------WAIT() calls------------------///
  //check if a process is sleeping and is ready to be scheduled now :)
  checkAwake();
  //------------------------------------------------///

  runnableTaskQ = moveTaskToEndOfList(runnableTaskQ);
  next_process = runnableTaskQ->task;
  printf("In schedule: Running Pid=%p, Next Pid=%p \n", running->pid, next_process->pid ); 
  //uint64_t tem = 0x28; 
  //Setting CR3 .. CHANGING PAGE TABLES
  _ptcr3(next_process->cr3);
  running = next_process;
  tss.rsp0 = (uint64_t) &(next_process->kernel_stack[255]);
  // __asm volatile("mov %0,%%rax;"::"r"(tem));
  // __asm volatile("ltr %ax");
  __asm volatile(
    "movq %0, %%rsp;\n\t"
    "popq %%r15\n\t"
    "popq %%r14\n\t"
    "popq %%r13\n\t"
    "popq %%r12\n\t"
    "popq %%r11\n\t" 
    "popq %%r10\n\t" 
    "popq %%r9\n\t"  
    "popq %%r8\n\t"
    "popq %%rbp\n\t"
    "popq %%rdi\n\t"
    "popq %%rsi\n\t"
    "popq %%rdx\n\t"
    "popq %%rcx\n\t"
    "popq %%rbx\n\t"
    "popq %%rax\n\t"
    :
    :"r"(&(next_process->kernel_stack[234]))
    :"memory"
  );
  __asm volatile(
    "iretq"
  );
}  


