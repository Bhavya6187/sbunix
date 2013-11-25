/*
* Code for Virtual Memory Manager
* 
*
*/

# include <stdio.h>
#include <sys/phy_mem.h>
#include <sys/page_table.h>
# include <sys/v_mem_manager.h>
# include <sys/task_management.h>
# include <sys/tarfs.h>
# include <sys/gdt.h>

uint64_t cur_VK;
uint64_t cur_PK;
extern void _ptcr3(uint64_t ); //setting cr3 register to kick start paging
//uint64_t cur_VK = (0x326000 + 0xffffffff80000000);		// Free Virtual Memory above Kernel starts from here

/*
* Initializes the Virtual Memory Manager
*/
void init_VM(uint64_t phfree)
{
	cur_VK = (phfree + (16 * 1024) + 0xffffffff7fffffff) ;
	cur_PK = 0x2097152;	// Starts at 2 MB mark (abhi confirm)
	printf("\n VK:%X:PK:%x", cur_VK, cur_PK);
}
//uint64_t pid_bitmap[32] = {0};
/*
* Kernel Malloc- Virtual Memory- Uses get_page() to get 4Kb physical pages
* Allocates memory only above PhysFree - for Kernel Data Structure
* param : No of Bytes needed by Kernel code (uint64_t)
* Global: cur_VK; strats from kernmem + Physfree
* return: void* pointer to the memory needed
*/

void *k_malloc(uint64_t no_bytes)
{
	uint64_t pt = NULL;
//	uint64_t 
// abhi add check for top address boundary condition	
	if (MAX_KERN <= (cur_VK + no_bytes))
	{
		//exit();
		printf("\n Kernel Virtual OVERSHOOT");
		return NULL;
	}
	pt = cur_VK;
	cur_VK += no_bytes;	// Increase current Free Virtual Memory above kernel pointer by the no of bytes asked by thread in Kernel	

	return (void *)pt;
}


/*
* Code for Virtual Memory Free called from within exit
* param: VM address to be freed 
* Return:
*/


/*
* Kernel Malloc- below kernmem + Physbase Virtual Memory- Uses get_page() to get 4Kb physical pages
* Allocates memory only below kernmem + PhysFree - for Processes mapping (sort of brk() equivelent 
* param : No of Bytes needed by Process (uint64_t)
* Global: cur_VK; strats from 2MB to Kernmem + Physbase:
* return: void* pointer to the memory needed on success else returns NULL pointer
*/

void *p_malloc(uint64_t no_bytes)
{
	uint64_t pt1 = NULL;
//	uint64_t base = 0x200000;	

	if ((cur_PK + no_bytes) < (0xffffffff80000000 + BASE))
	{
		pt1 = cur_PK;
		cur_PK += no_bytes;
	}	
	else
	{
		return (void *) 0;	// Error Condition
	}	
	return (void *) pt1;
}

/*
* This function reads the segments from the elf binary and maps them at the given V-address backed by Physical Pages 
* which are allocated on the page fault. (The page fault handler calls the self refrence() function and maps a physical page to faulting Virtual address
* param: start_Vadd elf Segment start Virtual address
* param: source_add
* param: f_size size of the elf segment (bytes)
* param: m_size size of segment in memory (bytes)
* return: 0 on success 1 on error
* NOTE: ** THIS FUNCTION SHOULD ONLY BE CALLED WHEN p_malloc() call before is successful **
*/

uint32_t m_map(uint64_t start_Vadd, uint64_t source_add, uint64_t f_size, uint64_t m_size)
{
	char *check = NULL, *source = NULL;
	uint64_t i = 0;
	if (f_size < 1)
	{
		return 1;
	}

	check = (char *) start_Vadd;
	source = (char *) source_add;
//	*check = 0;						// abhi Will try to derefrence and trigger a Page Fault if Physical page is not mapped.

/* Page fault handler will be called which should use Self Refrencing to allocate a new Physical page to the process and return back to the next instruction */

	for (i = 0; i < m_size; i++)
	{
		if ( i > f_size && i < m_size)		// Need to zero out contents of page in this range
		{
			*check++ = 0;
		}
		else
		{
			*check++ = *source++;	// Load(copy) the contents of elf segments in memory at Virtual adress specified in elf byte by byte
		}
	}

	return 0;
}

/*
* Maps the kernel Page Tables in Process Page Table by copying kernel PML4E 511 entry into Process Page Table 
* Also creates Page Table for the new process 
* returns: uint64_t base Physical Address of the PML4e page table for the new process 
*/

uint64_t map_pageTable()
{
	uint64_t *p1, *tmp, *tmp1, *tmp2;
	uint64_t add = 0xFFFF000000000000;	// Base address to build on
	p1 = (uint64_t *) allocate_free_phy_page();
	
	if (p1 == NULL)
	{
		return NULL;
	}	
	
/* Looking into kernel page table */	
	add = (((add >> 48) << 9 | 0x1FE) << 39);  // Sets the 1st 9 bits to 510 for  self refrenccing
	add = (((add >> 39) << 9 | 0x1FE) << 30);  // Sets the 2nd 9 bits to 510 for  self refrenccing
	add = (((add >> 30) << 9 | 0x1FE) << 21);  // Sets the 3rd 9 bits to 510 for  self refrenccing
	add = (((add >> 21) << 9 | 0x1FE) << 12);  // Sets the 4th 9 bits to 510 for  self refrenccing
	tmp1 = (uint64_t *) add;

	add = 0xFFFF000000000000;
	add = (((add >> 48) << 9 | 0x1FE) << 39);  // Sets the 1st 9 bits to 510 for  self refrenccing
	add = (((add >> 39) << 9 | 0x1FE) << 30);  // Sets the 2nd 9 bits to 510 for  self refrenccing
	add = (((add >> 30) << 9 | 0x1FE) << 21);  // Sets the 3rd 9 bits to 510 for  self refrenccing
	add = (((add >> 21) << 9 | 0x1FD) << 12);  // Sets the 4th 9 bits to 509 to point to the extra page used to init the new process page table
	tmp = (uint64_t *) add;

/* Mapping New Process Page table at the tmp page(kernel PML4e[509]s 0th index */
	*(tmp + 0) = (((uint64_t) p1) | 7);
	 
	add = 0xFFFF000000000000;
	add = (((add >> 48) << 9 | 0x1FE) << 39);  // Sets the 1st 9 bits to 510 for  self refrenccing
	add = (((add >> 39) << 9 | 0x1FE) << 30);  // Sets the 2nd 9 bits to 0 for  self refrenccing
	add = (((add >> 30) << 9 | 0x1FD) << 21);  // Sets the 3rd 9 bits to 509  entry for  self refrenccing
	add = (((add >> 21) << 9 | 0x0) << 12);    // Sets the 4th 9 bits to 509 to point to the extra page used to init the new process page table
	tmp2 = (uint64_t *) add;

	*(tmp2 + 511) = (uint64_t)tmp1[511];	   // Mapping Kernel PML4e entry into process 
	*(tmp2 + 510) = (((uint64_t) p1) | 7);	  // Self Refrencing Trick	
	
	return (uint64_t) p1;			   // returns the PML4E base Physical address
}


/*
* Creates User mode dynamic stack for the process through Page Faults and Self Refrencing
* Returns: Pointer where the stack starts
* NOTE ** Stack grows downwards i.e.from high address to low addresses
*/
// abhi make it dynamic
uint64_t *process_stack()
{
	uint64_t *st = NULL;
	uint64_t *top = NULL;

	st = (uint64_t *) p_malloc(4096);
	if (st == NULL)
	{
		printf("\n Cant allocate Process Stack");
		//exit();
		return (void *)0;
	}

	top = (uint64_t *) (st + 4096);

	return top;	//returns the top of the virtual page 4KB page as stack grows downwards
}	


void test()	// sort of execve in current scenario
{
	PCB *pro = NULL;
  printf("Made the pcb");
	pro = create_pcb();
	_ptcr3(pro->cr3);
	read_tarfs(pro);
	printf("\n BACK IN TEST");
	if ((pro->u_stack = process_stack()) == NULL)
	{
		printf("\n Cant allocate memory for process User stack");
		//exit();
	}	
	/* Setting the CR3 with the new process PML4E */	
	printf("\n CR3 set done");
	/* Running new process */
	
//	running = pro;	// Pointer which keeps track of currently running process	
/*	pro->kernel_stack[63] = pro->rip;
	pro->rsp = (uint64_t)&(pro->kernel_stack[63]);
	__asm__(
	   "movq %0, %%rsp;"
	   :
	   :"r"((pro->rsp))
	);

	__asm__(
	   "retq;"
	);
*/
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
}
