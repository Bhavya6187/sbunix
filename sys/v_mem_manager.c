//Code for Virtual Memory Manager

#include<stdio.h>
#include<sys/phy_mem.h>
#include<sys/page_table.h>
#include<sys/v_mem_manager.h>
#include<sys/task_management.h>
#include<sys/tarfs.h>
#include<sys/gdt.h>

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

uint64_t selfRef(uint64_t pml4e, uint64_t pdpe, uint64_t pde, uint64_t pte)
{
  uint64_t base;
  base = 0xFFFF000000000000; 
  base = (((base >> (12+9+9+9+9))<<9 | pml4e ) << (12+9+9+9) );
  base = (((base >> (12+9+9+9))<<9   | pdpe  ) << (12+9+9) );
  base = (((base >> (12+9+9))<<9     | pde   ) << (12+9) );
  base = (((base >> (12+9))<<9       | pte   ) << (12) );
  return base;
}

/*
* Maps the kernel Page Tables in Process Page Table by copying kernel PML4E 511 entry into Process Page Table 
* Also creates Page Table for the new process 
* returns: uint64_t base Physical Address of the PML4e page table for the new process 
*/

uint64_t map_pageTable(PCB *np)
{
	uint64_t *p1, *tmp, *tmp1;
	p1 = (uint64_t *) allocate_free_phy_page();
	
	if (p1 == NULL)
	{
		return NULL;
	}	
	
  //Looking into kernel page table
  tmp1 = (uint64_t *)(selfRef(0x1FE, 0x1FE, 0x1FE, 0x1FE)); // 510

  uint64_t i=509;
  while( *(tmp1+i) )
  { 
    i--;
  }
  *(tmp1+i) = (((uint64_t) p1) | 7);

  tmp = (uint64_t *)(selfRef(0x1FE, 0x1FE, 0x1FE, i));  // 509

  //Mapping New Process Page table at the tmp page(kernel PML4e[509]s 0th index */
	//*(tmp + 0) = (((uint64_t) p1) | 7);

  //tmp2 = (uint64_t *)(selfRef(0x1FE, 0x1FE, 0x1FD, 0x0)); // 510 510 509 0
  //tmp2 = (uint64_t *)(selfRef(0x1FE, 0x1FE, 0x1FE, 0x1FD)); // 510 510 510 509

	*(tmp + 511) = (uint64_t)tmp1[511];	   // Mapping Kernel PML4e entry into process 
	*(tmp + 510) = (((uint64_t) p1) | 7);	 // Self Refrencing Trick	
  np->pml4e_entry = i;
  np->cr3 = (uint64_t) p1;
	
	return (uint64_t) p1;			   // returns the PML4E base Physical address
}

#define  R0 0xFFFFFFFFFFFFFFD //1 bit set as read bit, so no write access
#define COW 0x008000000000000 //52 bit set as COW bit
// Copying pagetables and setting COW bits for the present physical pages !!
void copyPageTables(PCB *child, PCB *parent)
{
  uint64_t i, j, k, l; // iterators for pml4e, pdpe, pde, pte
  uint64_t *pml4eAdd, *pdpeAdd, *pdeAdd, *pteAdd;
  uint64_t *new_pml4eAdd, *new_pdpeAdd, *new_pdeAdd, *new_pteAdd;
  uint64_t child_pml4e_entry;
  uint64_t total_count=0, tp1=0, tp2=0, tp3=0;
  
  child_pml4e_entry = child->pml4e_entry;
  printf("Child PML4E entry =%p\n", child_pml4e_entry);
  pml4eAdd = (uint64_t *)(selfRef(0x1FE, 0x1FE, 0x1FE, 0x1FE));
  new_pml4eAdd = (uint64_t *)(selfRef(0x1FE, 0x1FE, 0x1FE, child_pml4e_entry ));
  printf("pml4e=%p, new=%p\n", pml4eAdd, new_pml4eAdd);

  // Iterate through all the page table entries in the PML4E of the Parent process 
  for(i=0; i<510; i++)
  {
    if( i==child_pml4e_entry)
      continue;

    if(pml4eAdd[i]!=0) // if some entry exists we have to copy
    {
      //new_pml4eAdd[i] = pml4eAdd[i];
	    new_pml4eAdd[i] = (((uint64_t) allocate_free_phy_page()) | 7);
      tp1++;
      pdpeAdd = (uint64_t *)(selfRef(0x1FE, 0x1FE, 0x1FE, i));
      new_pdpeAdd = (uint64_t *)(selfRef(0x1FE, 0x1FE, child_pml4e_entry, i));
      printf("pdpe=%p, new=%p\n", pdpeAdd, new_pdpeAdd);
      
      for( j=0; j<512; j++)
      {
        if(pdpeAdd[j])
        {
          //new_pdpeAdd[j] = pdpeAdd[j];
	        new_pdpeAdd[j] = (((uint64_t) allocate_free_phy_page()) | 7);
          tp2++;
          pdeAdd = (uint64_t *)(selfRef(0x1FE, 0x1FE, i, j));
          new_pdeAdd = (uint64_t *)(selfRef(0x1FE, child_pml4e_entry, i, j));
          printf("pde=%p, new=%p\n", pdeAdd, new_pdeAdd);

          for( k=0; k<512; k++)
          {
            if(pdeAdd[k])
            {
              //new_pdeAdd[j] = pdeAdd[j];
	            new_pdeAdd[k] = (((uint64_t) allocate_free_phy_page()) | 7);
              tp3++;
              pteAdd = (uint64_t *)(selfRef(0x1FE, i, j, k));
              new_pteAdd = (uint64_t *)(selfRef(child_pml4e_entry, i, j, k));
              printf("pte=%p, new=%p\n", pteAdd, new_pteAdd);

              for(l=0; l<512; l++)
              {
                if(pteAdd[l])
                {
                  // setting COW 52 bit as 1 and give read only access to the physical page
                  // for both parent and child
                  new_pteAdd[l] = ((((uint64_t)pteAdd[l]) & R0) | COW);
                  pteAdd[l]     = ((((uint64_t)pteAdd[l]) & R0) | COW);
                  total_count++;
                }
              }
            }
          }

        }
      }
    }
  }
  printf("Total no of entries finally added in PTE's :: %p",total_count);
  printf("Total no of pages allocated for PageTables :: %p %p %p",tp1, tp2, tp3);
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
  uint64_t i=0;
  while(i<8)
  {
    st = (uint64_t *) p_malloc(4096);
	  if (st == NULL)
	  {
		  printf("\n Cant allocate Process Stack");
		  //exit();
		  return (void *)0;
	  }
    i++;
  }

	top = (uint64_t *) (st + 4096);

	return top;	//returns the top of the virtual page 4KB page as stack grows downwards
}	


void test()	// sort of execve in current scenario
{
	PCB *pro = NULL;
  printf("Made the pcb");
	pro = create_pcb();
 	if ((pro->pid = get_pid()) == 0)
	{
		printf("\n Error No Free PID found");	
	}
	pro->cr3 = map_pageTable(pro);		// Storing Base Physical address of PML4e for new process
	printf("\n PCR3:%x", pro->cr3);
  //Adding the PCB into Queue of Running process
  allTaskQ = addToHeadTaskList(allTaskQ, pro); 


  pro->ppid=0;
	_ptcr3(pro->cr3);
  char elf_file[10]="bin/hello";
	read_tarfs(pro,elf_file);
	printf("\n BACK IN TEST");
	if ((pro->u_stack = process_stack()) == NULL)
	{
		printf("\n Cant allocate memory for process User stack");
		//exit();
	}	
	/* Setting the CR3 with the new process PML4E */	
	printf("\n CR3 set done");
  runnableTaskQ = addToHeadTaskList(runnableTaskQ, pro); 
  running = pro;
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
	tss.rsp0 = (uint64_t)  &(pro->kernel_stack[511]);
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
