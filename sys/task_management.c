// Code for Task/Process Management

# include <sys/task_management.h>
# include <sys/task_schedule.h>
# include <sys/page_table.h>
# include <sys/tasking.h>
# include <sys/v_mem_manager.h>
# include <stdio.h>
# include <sys/gdt.h>
# include <sys/irq.h>
# include <sys/tarfs.h>

extern void _ptcr3(uint64_t ); //setting cr3 register to kick start paging
struct taskList *waitTaskQ;
struct taskList *allTaskQ;
struct taskList *runnableTaskQ;
int no_waitQ=0, no_allQ=0, no_runnableQ=0;
PCB * running;
uint64_t pid_bitmap[32] = {0};
extern uint64_t id;
uint64_t id =0;
//Returns a free pid (32 bit unsigned integer for the new process)
uint64_t get_pid()
{
  uint64_t i = 1;
	while (pid_bitmap[i++] != 0 && i < MAXPID);	// abhi convert that to bitmap with bit manipulation
	if (MAXPID < i && pid_bitmap[MAXPID] != 0)
	{
		return 0;				// 0 in this case shows error that is no free PID found abhi what to do in this case
	}
	pid_bitmap[i-1] = 1;
	return (i-1);
}

struct taskList *addToHeadTaskList(struct taskList *list, struct pcb_t *new_pcb)
{
    struct taskList *temp = (struct taskList *)k_malloc(sizeof(struct taskList));
    temp->task = new_pcb;
    list->prev = temp;
    temp->next = list;
    temp->prev = NULL;
    return temp;
}

struct taskList *addToTailTaskList(struct taskList *list, struct pcb_t *new_pcb)
{
    struct taskList *temp = (struct taskList *)k_malloc(sizeof(struct taskList));
    struct taskList *start = list;
    temp->task = new_pcb;
    list->prev = temp;
    temp->next = list;
    temp->prev = NULL;
    
    temp->next = NULL;
    temp->task = new_pcb;
    if (list == NULL)
    {
      temp->prev = NULL;
      list = temp;
    }   
    else
    {
      while (list->next)
        list = list->next;
      temp->prev = list;
      list->next = temp;
    }
    return start;
}

struct taskList *removeFromTaskList(struct taskList *list, struct pcb_t *rem_pcb)
{
    if (list == NULL) 
    {
        printf("Fault.. No tasks exists.. !!\n");
        while(1);
    }
    //struct taskList *temp;

    if (list->task == rem_pcb) 
    {
        //temp = list;
        // Deallocate(temp) memories for the process  dushyant 
        list = list->next;
        list->prev = NULL;
        return list;
    } 
    else 
    {
        while (list->next->task != rem_pcb)
          list = list->next;
        if (list->task == rem_pcb) 
        {
          //temp = list;
          // Deallocate(temp) memories for the process  dushyant 
          list->prev->next = list->next;
          list->next->prev = list->prev;
          return list;
        }
    }
    return NULL;
}

struct taskList *moveTaskToEndOfList(struct taskList *list)
{
    if(list==NULL)
    {
        return list;
    }
    if (list->next == NULL)
    {
        return list;
    }
    struct taskList *start = list;
    struct taskList *temp = list;
    list = list->next;
    list->prev = NULL;
    while (temp->next)
        temp = temp->next;
    temp->next = start;
    start->next = NULL;
    start->prev = temp;
    return list;
}

uint64_t get_curr_PID()
{
  // returning the pid from the current task
  return (uint64_t)allTaskQ->task->pid;
  //return runnableTaskQ->task->pid;
}

uint64_t get_PID_PCB(struct pcb_t * gpcb)
{
  // returning the pid from the current task
  struct taskList * temp;
  temp = allTaskQ;
  if(temp==NULL)
  {
    printf("This PCB doesnt exist\n");
    return -1;
  }
  while(temp->next !=NULL)
  {
    if(temp->task == gpcb)
      return temp->task->pid;
    temp=temp->next;
  }
  return -1;
  //return runnableTaskQ->task->pid;
}

PCB *get_curr_PCB()
{
  //return (int)allTaskQ->task;
  return runnableTaskQ->task;
}

PCB *get_parent_PCB(uint64_t parent_pid)
{
  //return (int)allTaskQ->task;
  struct taskList * temp;
  temp = allTaskQ;
  if(temp==NULL)
  {
    printf("NO PCB exist in allTasksQ\n");
    return NULL;
  }
  while(temp->next !=NULL)
  {
    if(temp->task->pid == parent_pid)
      return temp->task;
    temp=temp->next;
  }
  return NULL;
}

// This function creates the PCB for each new process created
PCB *create_pcb()
{
	PCB *pro = NULL;

	pro = (PCB *) k_malloc(sizeof(PCB));
	if (pro == NULL)
	{
		printf("\n Can't Allocate Memory for PCB");
		return (PCB *) 0;
	}
  allTaskQ = addToTailTaskList(allTaskQ, pro);
  no_allQ++;
	
	/* Update Values in the PCB sturct for the new process */

  /*
	if ((pro->pid = get_pid()) == 0)
	{
		printf("\n Error No Free PID found");	
		return (PCB *) 0;
	}
	pro->cr3 = map_pageTable();		// Storing Base Physical address of PML4e for new process
	printf("\n PCR3:%x", pro->cr3);

  //Adding the PCB into Queue of Running process
  */

  //	pro->mm_st = create_vma();
	return pro;
}		
		
// This code creates the VMA for each segments of the elf binary
VMA *create_vma(uint64_t start_add, uint64_t size)
{
	VMA *vm = NULL;

	vm = k_malloc(sizeof(VMA));
	if (vm == NULL)
	{
		//exit();
		printf("\n Cant allocalte memory for VMA exit now");
	}
	vm->vma_start_add = start_add;
	vm->vma_end_add = (start_add + size);
	vm->vma_next = NULL;

	return vm;
}

// Fork() Creating a child process from a parent
uint64_t doFork()
{
	PCB *pro = NULL;
	PCB *parent_process;
  //parent_process = get_curr_PCB();
  parent_process = running;
	pro = create_pcb();

  m_map((uint64_t)pro, (uint64_t)(get_curr_PCB()), (uint64_t)(sizeof(struct pcb_t)), (uint64_t)(sizeof(struct pcb_t)) );
 
  // Getting new pid -------------------------------------------------------------------------
  if ((pro->pid = get_pid()) == 0)
	{
		printf("\n Error No Free PID found");	
		return -1;
	}
	pro->cr3 = map_pageTable(pro);		   // Storing Base Physical address of PML4e for new process
  pro->ppid = parent_process->pid;
	printf("\n PCR3:%x", pro->cr3);
  pro->cow = 1;
  parent_process->cow = parent_process->cow + 1;
  /// -----------------------------------------------------------------------------------------

  //copy the page tables of parent process !!
  //copyUST(pro);
  checkUST(pro);
  checkUST(running);
  copyPageTables(pro, parent_process);
  printf("PageTable copying done\n");

  printf("Child PID=%p\n", pro->pid);
  printf("Parent PID=%p\n", pro->ppid);
  printf("Current PID=%p\n", get_curr_PID());
  printf("Current Process=%p \n", get_curr_PCB());
  printf("Current Process from RUNNING=%p \n", running);
  printf("CP Cr3=%p Chid Cr3=%p\n", running->cr3, pro->cr3);
  // Add it to task linked list !!
  runnableTaskQ = addToTailTaskList(runnableTaskQ, pro); 
  no_runnableQ++;
  // return 0 to the calling process
  //m_map( (uint64_t)pro->u_stack, (uint64_t)(parent_process->u_stack), (uint64_t)(4096), (uint64_t)(4096) );
	printf("\n Trying to Fork()\n");
  //copyOnWritePageTables();
  // update the cr3 to process->cr3
  /*if ((pro->u_stack = process_stack()) == NULL)
	{
		printf("\n Cant allocate memory for process User stack");
		//exit();
	}*/
  //check this function
  //m_map((uint64_t)pro->u_stack, (uint64_t)parent_process->u_stack, (uint64_t)(4096*8), (uint64_t)(4096*8) );
  /*  
  pro->u_stack[0] = pro->rip;
	pro->rsp = (uint64_t)(pro->u_stack);
	tss.rsp0 = (uint64_t)  &(pro->kernel_stack[63]);
	printf("\n GDT SET");
	uint64_t tem = 0x28; 
	__asm volatile("mov %0,%%rax;"::"r"(tem));
	__asm volatile("ltr %ax");
	__asm volatile("\
	push $0x23;\
	push %0;\
	pushf;\
	push $0x1B;\
	push %1"::"g"(&pro->u_stack[0]),"g"(pro->rip):"memory");
	__asm volatile("\
	iretq;\
	");
  */
  //Add parent_pid in the structure as well
 // if (test)
 // {
  //    printf("I am in child");
 //     goto label:
 // }
 //pro->rip = 0xffffffff80202508;      
// label:
  /*__asm volatile(
		"movq %0, %%rsp;"
		:
		:"r"((pro->rsp))
	        :"memory"
	);*/
  
  // Setting up the rax for both parent and child
  GREG *pp1 = (GREG *) &(running->kernel_stack[235]);
  GREG *cc1 = (GREG *) &(pro->kernel_stack[235]);
  printf("\n TE:%x:%x", pp1->rax, cc1->rax);
  pp1->rax = pro->pid;
  cc1->rax = 0x0; 
  printf("\n TE:%x:%x", pp1->rax, cc1->rax);
  if (running->pid == pro->ppid)
  {
    printf("This is the parent ! Returning with pid=%p, parent/running pid=%p\n", pro->pid, running->pid);
    //Set everything for our child for it to execute in its registers
    //parent_process->
	  //__asm volatile("sti");
    //while(1);
    return pro->pid;
  }
  else
  {
    printf("This is the CHILD !!\n");
	  //__asm volatile("sti");
    return 0;
  }
  //printf("Returning from fork() .. pid=%p, running pid=%p\n", pid, running->pid);

}


// Fork() Creating a child process from a parent
//void doExec(char* filename, char* argv, char *en[])
void doExec(char* filename)
{
 
  PCB *pro;
  pro = running;

  // delete all page table entries
  deletePageTables();
  
  printf("In Exec :: check PID=%p, cr3=%p ppid=%p\n", pro->pid, pro->cr3, pro->ppid);
	
  // Hope CR3 is already switched
  //_ptcr3(pro->cr3);
  
  // ------------------------------------------- Zero out the kernel stack as well
  int i=0;
  for(i=0; i<256; i++)
    pro->kernel_stack[i]=0x0;

  //char elf_file[10]="bin/world";
  //char elf_file[10]="bin/bash";
	//read_tarfs(pro,elf_file);
	read_tarfs(pro, filename);
	printf("We will execute - %s\n", filename);
	//printf("We will execute - %s\n", elf_file);
	if ((pro->u_stack = process_stack()) == NULL)
	{
		printf("\n Cant allocate memory for process User stack");
		//exit();
	}	
  
	pro->u_stack[0] = pro->rip;
	pro->rsp = (uint64_t)(pro->u_stack);
  printf("user_stack_rsp%p",pro->rsp);
	tss.rsp0 = (uint64_t)  &(pro->kernel_stack[255]);
	printf("In Execve()  GDT SET\n");
	uint64_t tem = 0x28; 
	__asm volatile("mov %0,%%rax;"::"r"(tem));
	__asm volatile("ltr %ax");
	__asm volatile("\
	push $0x23;\
	push %0;\
	pushf;\
	push $0x1B;\
	push %1"::"g"(&pro->u_stack),"g"(pro->rip):"memory");
	__asm volatile("\
	iretq;\
  ");

}


void exit_process(int status)
{
  printf("Exit() called for process pid=%p", running->pid);

  // Free the memory used by the process
  deletePageTables();

  // Bring parent from the waitlist because in waitpid the parent might be waiting for the child to finish
  struct taskList * temp;
  temp = waitTaskQ;
  if(temp==NULL)
    printf("There are no processes in the WaitQ strange ???\n");
  while(temp->next !=NULL)
  {
    if(temp->task->pid == running->ppid) // parent found in the waitQ --- move it to the runnableQ 
    {
      waitTaskQ = removeFromTaskList(waitTaskQ, temp->task);
      runnableTaskQ = addToTailTaskList(runnableTaskQ, temp->task);
      //runnableTaskQ = addToHeadTaskList(runnableTaskQ, temp->task);
      break;
    }
    temp=temp->next;
  }

  // REmove the process from the Queue
  runnableTaskQ = removeFromTaskList(runnableTaskQ, running);

  // call schedule() to start other process
  //schedule()
}


// The parent will wait for the child pid to exit() before continuing execution
void wait_pid(uint64_t pid)
{
  // just remove current running parent process to waitQ from runnableQ
  waitTaskQ = addToTailTaskList(waitTaskQ, running);
  runnableTaskQ = removeFromTaskList(runnableTaskQ, running);
  // parent will pause executing and call yield
  
}

// The parent will wait for the child pid to exit() before continuing execution
void wait_p()
{
  printf("In waitp() process pid=%p", running->pid);

  // Bring parent from the waitlist because in waitpid the parent might be waiting for the child to finish
  struct taskList * temp;
  temp = waitTaskQ;
  if(temp==NULL)
    printf("There are no processes in the WaitQ strange ???\n");
  while(temp->next !=NULL)
  {
    if(temp->task->ppid == running->pid) // child found in the runnableQ --- move parent to the waitQ until one child exits 
    {
      waitTaskQ = removeFromTaskList(waitTaskQ, temp->task);
      runnableTaskQ = addToTailTaskList(runnableTaskQ, temp->task);
      //runnableTaskQ = addToHeadTaskList(runnableTaskQ, temp->task);
      break;
    }
    temp=temp->next;
  }
}

// Sleeps for a specified number of seconds (time in secs)
// sec is a global variable which has count of number of seconds elapsed 
void sleep(uint64_t time)
{

  running->sleep_start = sec;
  running->sleep_duration = time;
  runnableTaskQ = removeFromTaskList(runnableTaskQ, running);
  waitTaskQ = addToTailTaskList(waitTaskQ, running);
  // if(sec - sleep_start > time) 
  //     do the following !! 

  // call schedule here to schedule another process
  // Make sure to check for the waiting processes to move them from waitQ to the runnableQ
  // schedule();

}

// Schedule function for context switching between two processes.. Hope it should work for both kernel and user processes
void scheduler1()
{
//  test += 1;
	PCB *current_process, *new_process;
  //printf("In scheduler :P\n");
  // 0 or 1 process
  /*if(no_runnableQ<2)
  {
    __asm volatile(
		  "retq;"
	  );
  }*/
  current_process = running;
  runnableTaskQ = moveTaskToEndOfList(runnableTaskQ);
  new_process = runnableTaskQ->task;
	// Should save state of caller here 
	__asm volatile(
		"cli"
		);
  __asm volatile(
		"push %rax"
		);
	__asm volatile(
		"push %rbx"
		);
	__asm volatile(
		"push %rcx"
		);
	__asm volatile(
		"push %rdx"
		);
	__asm volatile(
		"push %rbp"
		);
	__asm volatile(
		"push %r8"
		);
	__asm volatile(
		"push %r9"
		);
	__asm volatile(
		"push %r10"
		);
	__asm volatile(
		"push %r11"
		);
	__asm volatile(
		"push %r12"
		);
	__asm volatile(
		"push %r13"
		);
	__asm volatile(
		"push %r14"
		);
	__asm volatile(
		"movq %%rsp, %0"
		:"=g"(current_process->rsp)
		:
	  :"memory"
	);
  //current_process->rip = (uint64_t)(*((uint64_t*)(current_process->rsp)+12));

	/* Now change the %rsp to callee rsp */
	__asm volatile(
		"movq %0, %%cr3;"
		:
		:"r"((new_process->cr3))
    :"memory"
	);

  //printf("cr3 old=%p new=%p\n", current_process->cr3, new_process->cr3);  
  //new_process->rsp = current_process->rsp;
  //new_process->rip = current_process->rip;
	
  /* Now change the %rsp to callee rsp */
	__asm volatile(
		"movq %0, %%rsp;"
		:
		:"r"((new_process->rsp))
	        :"memory"
	);
	__asm volatile(
		"pop %r14"
		);
	__asm volatile(
		"pop %r13"
		);
	__asm volatile(
		"pop %r12"
		);
	__asm volatile(
		"pop %r11"
		);
	__asm volatile(
		"pop %r10"
		);
	__asm volatile(
		"pop %r9"
		);
	__asm volatile(
		"pop %r8"
		);
	__asm volatile(
		"pop %rbp"
		);
	__asm volatile(
		"pop %rdx"
		);
	__asm volatile(
		"pop %rcx"
		);
	__asm volatile(
		"pop %rbx"
		);
	__asm volatile(
		"pop %rax"
		);

  //tss.rsp0 = (uint64_t)  &(new_process->kernel_stack[63]);
	//printf("\n GDT SET");
	
  /*
  uint64_t tem = 0x2B; 
	__asm volatile("mov %0,%%rax;"::"r"(tem));
	__asm volatile("ltr %ax");*/
/*	__asm volatile("\
	push $0x23;\
	push %0;\
	pushf;\
	push $0x1B;\
	push %1"::"g"(new_process->rsp),"g"(new_process->rip):"memory");
	
  __asm volatile("\
	iretq;\
	");
*/
	__asm volatile(
		"iretq;"
	);	
}
