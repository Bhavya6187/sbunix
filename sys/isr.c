#include <sys/idt.h>
#include <sys/isr.h>
#include <sys/page_table.h>
#include <sys/task_management.h>
#include <sys/task_schedule.h>
#include <sys/v_mem_manager.h>
#include <stdio.h>
#include <sys/dirent.h>
#include <sys/string.h>
    
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
    while(1);
}

void isr_handler_14(registers_t regs)
{
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
    //printf("Interrupt Number = %d %d %p\n", 14, regs.err_code, cr2);
    if(re==7)
    {
      //printf("RE=7\n");
      phyPageEntry = (uint64_t)getPhyFromVirtual((uint64_t)cr2);
      if(phyPageEntry==0)
      {
        //printf("Virtual Address doeant exist");
        // Do page mapping probably !!
      }
      check_COW =  (( ((uint64_t)phyPageEntry)  & COW)>>51) ;
      //printf("IN = %d %d %p phypageentry %d %p\n", 14, regs.err_code, cr2, check_COW, phyPageEntry);
      if(check_COW)
      {
        //printf("COW bit is 1 i.e. fork() has been done for this process\n");
        copyOnWritePageTables();
        _ptcr3(running->cr3);
        //Allocate new pages for the child or the parent.. yet to be done
      }
    }
    else
    {
      if ((0 == re) || (2 == re) || (4 == re) || (6 == re))
        page_mapping(cr2);
    }
    //printf("Exception: Page Fault Handeled !! \n");
}

uint64_t isr_handler_80(myregs_t *regs )
{
   uint64_t ret = 0;
   uint64_t num_bytes = 0;
   uint64_t f=-2, str = 0, filename = 0,fd=0, params=0;
   uint64_t exit_status=0, pid, time_secs;
   dirent* dir;
   dirent* dir_init;
   DIR* directory_struct;
   DIR* directory_init;
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
        ret = write(1,(char *)addr,num);
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
        addr = regs->rdx;
        str = regs->rcx;
        fd = regs->rbx;
        if(fd ==0)
          ret =(uint64_t)scanf((char*)str,addr);
        else
          ret =(uint64_t)read_file(fd,str,(char*)addr);
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
      case(12):
        directory_init =(DIR*)regs->rbx;
        dir =(struct dirent*) regs->rcx;
        dir_init = (struct dirent*)k_malloc(sizeof(struct dirent));
        dir_init = readdir((DIR*)directory_init);
        
        if(dir_init!=NULL)
        {
          regs->rax = 1;
          strcpy(dir->d_name,dir_init->d_name);
          dir->offset = dir_init->offset;
        }
        else
          regs->rax = 0;
        break;
        if(dir!=NULL)
          regs->rax = (uint64_t)dir;
        else
          regs->rax = 0;
        break;
      case(13):
        str = regs->rbx;
        directory_init = (DIR*)regs->rcx;

        directory_struct = opendir((char*)str);

        if(directory_struct!=NULL)
        {
          regs->rax = 1;
          strcpy(directory_init->dirname,directory_struct->dirname);
          directory_init->current = directory_struct->current;
          directory_init->dirent_filled = directory_struct->dirent_filled;
        }

        else
          regs->rax = 0;
        break;

      case(14):
        directory_init = (DIR*)regs->rbx;
        ret = closedir(directory_init);
        regs->rax = ret;
        break;
      case(15):
        addr = regs->rbx;
        num = regs->rcx;
        fd=2;
        ret = write(fd,(char *)addr,num);
        regs->rax = ret;
        break;
      case(16):
        str = regs->rbx;
        ret = open_file((char*)str);
        regs->rax = ret;
        break;
      case(17):
        fd = regs->rbx;
        ret = close_file(fd);
        regs->rax = ret;
        break;
      case(18):
        filename = regs->rbx;
        params = regs->rcx;
        doExecvp((char*)filename, (char**)params);
        break;
      case(19):
        ret = get_curr_PID();
        regs->rax = ret;
        break;
      case(20):
        PS_Display();
        break;
      case(21):
        pid = regs->rbx;
        ret = kill_process(pid);
        regs->rax = ret;
        break;
        
      default:
        return 0;
    }
    return ret;
    // printf("Interrupt 80 for system calls .... \n");
}
