#include <sys/idt.h>
#include <sys/isr.h>
#include <sys/page_table.h>
#include <sys/task_management.h>
#include <sys/task_schedule.h>
#include <sys/v_mem_manager.h>
#include <stdio.h>
    
#define COW 0x0008000000000000 //52 bit set as COW bit

void isr_handler_0(registers_t regs)
{
     printf("Error: Divide by zero.. !! Grrr.... \n");
     // Pushed by pushq i.e. all general purpose registers
     //printf(" r15 %x, r14 %x, r13 %x, r12 %x, r11 %x, r10 %x, r9 %x, r8 %x,\n rsp %x, rbp %x, rdi %x, rsi %x, rdx %x, rcx %x, rbx %x, rax %x \n",     regs.r15, regs.r14, regs.r13, regs.r12, regs.r11, regs.r10, regs.r9, regs.r8, regs.rsp, regs.rbp, regs.rdi, regs.rsi, regs.rdx, regs.rcx, regs.rbx, regs.rax);
    //regs.interrupt_number = 0;
     printf("Interrupt Number = %d %d\n", 0, regs.err_code);
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
    //regs.interrupt_number = 13;
    printf("Interrupt Number = %d errCode=%d cr=%p\n", 13, regs.err_code, cr2);
    printf("Success !!\n");
    while(1);
}

void isr_handler_14(registers_t regs)
{
     printf("Exception: Page Fault !! Lol .. !! Grrr.... \n");
     // Pushed by pushq i.e. all general purpose registers
     // printf(" r15 %x, r14 %x, r13 %x, r12 %x, r11 %x, r10 %x, r9 %x, r8 %x,\n rsp %x, rbp %x, rdi %x, rsi %x, rdx %x, rcx %x, rbx %x, rax %x \n",     regs.r15, regs.r14, regs.r13, regs.r12, regs.r11, regs.r10, regs.r9, regs.r8, regs.rsp, regs.rbp, regs.rdi, regs.rsi, regs.rdx, regs.rcx, regs.rbx, regs.rax);
    uint64_t cr2;
    uint64_t check_COW=0;
    uint64_t phyPageEntry;
     //Read cr3 here -
  	__asm volatile(
		"movq %%cr2, %0"
		:"=g"(cr2)
		:
	  :"memory"
	  );
    int re=0; 
    //regs.interrupt_number = 14;
    re = (regs.err_code & 7);
    printf("Interrupt Number = %d %d %p\n", 14, regs.err_code, cr2);
    if(re==7)
    {
      printf("RE=7\n");
      phyPageEntry = (uint64_t)getPhyFromVirtual((uint64_t)cr2);
      if(phyPageEntry==0)
      {
        printf("Virtual Address doeant exist");
        // Do page mapping probably !!
      }
      check_COW =  (( ((uint64_t)phyPageEntry)  & COW)>>51) ;
      printf("IN = %d %d %p phypageentry %d %p\n", 14, regs.err_code, cr2, check_COW, phyPageEntry);
      if(check_COW)
      {
        printf("COW bit is 1 i.e. fork() has been done for this process\n");
        copyOnWritePageTables();
        //Allocate new pages for the child or the parent.. yet to be done
      }
    }
    else
    {
      if ((0 == re) || (2 == re) || (4 == re) || (6 == re))
        page_mapping(cr2);
    }
    printf("Success !!\n");
}

uint64_t isr_handler_80(myregs_t *regs )
{
   uint64_t ret = 0;
   uint64_t num_bytes = 0;
   uint64_t f=-2, str = 0, filename = 0;
   uint64_t exit_status=0, pid, time_secs;
   f++;
   //__asm__ volatile("movq %%rdi,%0;":"=m"(rdi1)::);
   //registers_t* regs = (registers_t*)rdi1;
    //regs.interrupt_number = 80;
    int n = regs->rax,var,num;
    uint64_t addr;
    switch(n){
      case(1):
        var =(int)regs->rbx;
        putint(var);
        break;
      case(2):
        addr = regs->rbx;
        num = regs->rcx;
        ret = puts_user((char *)addr,num);
        regs->rax = ret;
        break;
      case(3):
        ret = doFork();
        regs->rax = ret;
        break;
      case(4):
        num_bytes = regs->rbx;
        addr =(uint64_t)p_malloc(num_bytes);
        regs->rax = addr;
        break;
      case(5):
        addr = regs->rcx;
        str = regs->rbx;
        ret =(uint64_t)scanf((char*)str,addr);
        regs->rax = ret;
        break;
      case(6):
        schedule_process();
        break;
      case(7):
        filename = regs->rbx;
        doExec((char*)filename);
        break;
      case(8):
        exit_status = regs->rbx;
        exit_process(exit_status);
        break;
      case(9):
        pid = regs->rbx;
        ret = wait_pid(pid);
        regs->rax = ret;
        break;
      case(10):
        ret = wait_p();
        regs->rax = ret;
        break;
      case(11):
        time_secs = regs->rbx;
        sleep_t(time_secs);
        break;

      default:
        return 0;
    }
    return ret;
    // printf("Interrupt 80 for system calls .... \n");
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

