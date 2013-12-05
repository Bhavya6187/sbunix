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
# include <sys/string.h>
# include <sys/errno.h>
# include <sys/tarfs.h>
# include <sys/dirent.h>


extern void _ptcr3(uint64_t ); //setting cr3 register to kick start paging
struct taskList *waitTaskQ = NULL;
struct taskList *allTaskQ = NULL;
struct taskList *runnableTaskQ = NULL;
struct taskList *deadTaskQ = NULL;

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
    temp->next = list;
    temp->prev = NULL;
    if(list!=NULL)
      list->prev = temp;
    return temp;
}

struct taskList *addToTailTaskList(struct taskList *list, struct pcb_t *new_pcb)
{
    struct taskList *temp = (struct taskList *)k_malloc(sizeof(struct taskList));
    struct taskList *start = list;
    //temp->task = new_pcb;
    //list->prev = temp;
    //temp->next = list;
    //temp->prev = NULL;
    
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

PCB *searchPCB(struct taskList *tlist, uint64_t pid)
{
    struct taskList *list = tlist;
    if(list==NULL)
      return NULL;
    if( (list->task->pid !=pid) && (list->next == NULL) )
      return NULL;
    list = list->next;
    while (list->next)
      if(list->next->task->pid == pid)
        return list->next->task;
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
  /// -----------------------------------------------------------------------------------------

  //copy the page tables of parent process !!
  //copyUST(pro);
  //checkUST(pro);
  //checkUST(running);
  //ckop();

  copyPageTables(pro, parent_process);
  printf("PageTable copying done\n");

  printf("Child PID=%p, pPID=%p\n", pro->pid, pro->ppid);
  printf("CP Cr3=%p Chid Cr3=%p\n", running->cr3, pro->cr3);
  // Add it to task linked list !!
  runnableTaskQ = addToTailTaskList(runnableTaskQ, pro); 
  no_runnableQ++;
  // return 0 to the calling process
  //m_map( (uint64_t)pro->u_stack, (uint64_t)(parent_process->u_stack), (uint64_t)(4096), (uint64_t)(4096) );
	printf("\n Trying to Fork()\n");
  
  //copyOnWritePageTables();
  _ptcr3(running->cr3);
  // update the cr3 to process->cr3
  /*if ((pro->u_stack = process_stack()) == NULL)
	{
		printf("\n Cant allocate memory for process User stack");
		//exit();
	}*/
  //check this function
  //m_map((uint64_t)pro->u_stack, (uint64_t)parent_process->u_stack, (uint64_t)(4096*8), (uint64_t)(4096*8) );
 
 // ckop();
 // _ptcr3(pro->cr3);
 // printf("hehehhe\n");
 // ckop();
 // _ptcr3(running->cr3);
  //while(1);
  
  // Setting up the rax for both parent and child
  GREG *pp1 = (GREG *) &(running->kernel_stack[234]);
  GREG *cc1 = (GREG *) &(pro->kernel_stack[234]);
  pp1->rax = pro->pid;
  cc1->rax = 0x0; 
  printf("\n TE:%x:%x", pp1->rax, cc1->rax);
  if (running->pid == pro->ppid)
  {
    printf("This is the parent ! Returning with pid=%p, parent/running pid=%p\n", pro->pid, running->pid);
    //Set everything for our child for it to execute in its registers
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

}

// Fork() Creating a child process from a parent
//void doExec(char* filename, char* argv, char *en[])
void doExec(char *filename)
{
 
  PCB *pro;
  pro = running;
  //char filename[40];
  //strcpy(filename, fn);
  // delete all page table entries
  deletePageTables();
  
  printf("In Exec :: check PID=%p, cr3=%p ppid=%p\n", pro->pid, pro->cr3, pro->ppid);
	
  // ------------------------------------------- Zero out the kernel stack as well
  /*
  int i=0;
  for(i=0; i<256; i++)
    pro->kernel_stack[i]=0x0;
  */

  
  //char *filename2;
  //filename2 = filename;
	//read_tarfs(pro, "bin/world");
	read_tarfs(pro, filename);
	printf("We will execute - %s\n", filename);
	if ((pro->u_stack = process_stack()) == NULL)
	{
		printf("\n Cant allocate memory for process User stack");
		//exit();
	}	
  
  _ptcr3(pro->cr3);
  
	//pro->u_stack[0] = pro->rip;
	pro->rsp = (uint64_t)(pro->u_stack);
  printf("user_stack_rsp%p",pro->rsp);
	tss.rsp0 = (uint64_t)  &(pro->kernel_stack[255]);
	printf("In Execve()  GDT SET\n");
	__asm volatile("\
	push $0x23;\
	push %0;\
	pushf;\
	push $0x1B;\
	push %1"::"g"(pro->u_stack),"g"(pro->rip):"memory");
	__asm volatile("\
	iretq;\
  ");

}



//////////////////////////////////////////////////////////////////////////////////////////

void exit_process(int status)
{
  printf("Exit() called for process pid=%p", running->pid);

  // Bring parent from the waitlist because in waitpid the parent might be waiting for the child to finish
  struct taskList * temp;
  temp = waitTaskQ;
  if(temp==NULL)
    printf("There are no processes in the WaitQ strange ???\n");
  while(temp)
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
  deadTaskQ = addToHeadTaskList(deadTaskQ, running);

  // Set the status value to the parent ??? Where I dont know it yet !! :P

  // Free the memory used by the process
  deletePageTables();

  // call schedule() to start other process
  schedule_process();
}


//waitpid(): on success, returns the process ID of the child whose state has changed; if WNOHANG was specified  and  one  or  more  child(ren)
//specified by pid exist, but have not yet changed state, then 0 is returned.  On error, -1 is returned.
// The parent will wait for the child pid to exit() before continuing execution
// The waitpid() system call suspends execution of the calling process until a child specified by pid argument has changed state.  By  default,
// waitpid() waits only for terminated children, but this behavior is modifiable via the options argument, as described below.
uint64_t wait_pid(uint64_t pid)
{
  // just remove current running parent process to waitQ from runnableQ
  // cheack if PID exits ??
  if( searchPCB(runnableTaskQ, pid)==NULL )
    return -1;
  
  waitTaskQ = addToTailTaskList(waitTaskQ, running);
  runnableTaskQ = removeFromTaskList(runnableTaskQ, running);

  // should i call schedule ??
  // parent will pause executing and call yield/schedule
  schedule_process();
  return 0;
}


// The parent will wait for the child pid to exit() before continuing execution
// wait(): on success, returns the process ID of the terminated child; on error, -1 is returned.
uint64_t wait_p()
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
  return 0;

}

void checkAwake()
{
  struct taskList *list = waitTaskQ;
  while(list)
  {
    if ( (sec - (list->task->sleep_start)) > list->task->sleep_duration )
    {
      runnableTaskQ = addToHeadTaskList(runnableTaskQ, list->task);
      waitTaskQ = removeFromTaskList(waitTaskQ, list->task);
    }
    list = list->next;
  }
}

// Sleeps for a specified number of seconds (time in secs)
// sec is a global variable which has count of number of seconds elapsed 
void sleep_t(uint64_t time)
{
  running->sleep_start = sec;
  running->sleep_duration = time;
  runnableTaskQ = removeFromTaskList(runnableTaskQ, running);
  waitTaskQ = addToTailTaskList(waitTaskQ, running);
  // if(sec - sleep_start > time) 
  //     do the following !! 

  // call schedule here to schedule another process
  // Make sure to check for the waiting processes to move them from waitQ to the runnableQ
  schedule_process();

}

