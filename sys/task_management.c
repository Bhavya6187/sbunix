// Code for Task/Process Management

# include <sys/task_management.h>
# include <sys/page_table.h>
# include <sys/tasking.h>
# include <sys/v_mem_manager.h>
# include <stdio.h>

extern void _ptcr3(uint64_t ); //setting cr3 register to kick start paging

struct taskList *waitTaskQ;
struct taskList *allTaskQ;
struct taskList *runnableTaskQ;

uint64_t pid_bitmap[32] = {0};

//Returns a free pid (32 bit unsigned integer for the new process)
uint32_t get_pid()
{
	uint32_t i = 1;
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

int get_curr_PID()
{
  // returning the pid from the current task
  return (int)allTaskQ->task->pid;
  //return runnableTaskQ->task->pid;
}

int get_PID_PCB(struct pcb_t * gpcb)
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
	
	/* Update Values in the PCB sturct for the new process */

	if ((pro->pid = get_pid()) == 0)
	{
		printf("\n Error No Free PID found");	
		return (PCB *) 0;
	}
	pro->cr3 = map_pageTable();		// Storing Base Physical address of PML4e for new process
	printf("\n PCR3:%x", pro->cr3);
	_ptcr3((uint64_t)pro->cr3);
	printf("\n Process CR3 switch done");

  //Adding the PCB into Queue of Running process
  //allTaskQ = addToTailTaskList(allTaskQ, pro);

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
	PCB *process = NULL;
	process = create_pcb();
  m_map((uint64_t)process, (uint64_t)(get_curr_PCB), (uint64_t)(sizeof(struct pcb_t)), (uint64_t)(sizeof(struct pcb_t)) );

  process->pid = get_pid();
  // Add it to task linked list !!
  // update the cr3 to process->cr3
  allTaskQ = addToTailTaskList(allTaskQ, process); 
  // return 0 to the calling process

  //Add parent_pid in the structure as well
  return 0;

}
