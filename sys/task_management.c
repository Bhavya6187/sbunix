/*
* Code for Task/Process Management
*
*
*/

# include <sys/task_management.h>
# include <sys/page_table.h>
# include <sys/tasking.h>
# include <sys/v_mem_manager.h>
# include <stdio.h>

extern void _ptcr3(uint64_t ); //setting cr3 register to kick start paging
uint64_t pid_bitmap[32] = {0};
/*
* Returns a free pid (32 bit unsigned integer for the new process)
*/
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


/*
* This function creates the PCB for each new process created
*
*
*/

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
		printf("\n Error No Free PID found");	// abhi abort ?
		return (PCB *) 0;
	}
	pro->cr3 = map_pageTable();		// Storing Base Physical address of PML4e for new process
	printf("\n PCR3:%x", pro->cr3);
	_ptcr3((uint64_t)pro->cr3);
	printf("\n Process CR3 switch done");

//	pro->mm_st = create_vma();
	return pro;
}		
		
/*
* This code creates the VMA for each segments of the elf binary
* 
*/

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
