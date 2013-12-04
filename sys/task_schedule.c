#include<sys/task_management.h>
#include<sys/v_mem_manager.h>
#include<sys/gdt.h>
#include<stdio.h>

void schedule_process()
{
	PCB  *next_process=NULL;
  // Add current process to runableQ
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
    "pop %%r15\n\t"
    "pop %%r14\n\t"
    "pop %%r13\n\t"
    "pop %%r12\n\t"
    "pop %%r11\n\t" 
    "pop %%r10\n\t" 
    "pop %%r9\n\t"  
    "pop %%r8\n\t"
    "pop %%rdi\n\t"
    "pop %%rsi\n\t"
    "pop %%rdx\n\t"
    "pop %%rcx\n\t"
    "pop %%rbx\n\t"
    "pop %%rax\n\t"
    :
    :"r"(&(next_process->kernel_stack[235]))
    :"memory"
  );
  __asm volatile(
    "iretq"
  );
}  


