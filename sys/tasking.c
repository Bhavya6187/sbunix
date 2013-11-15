/* Implements process management  */

# include <stdio.h>
# include <defs.h>
# include <sys/tasking.h>
#include <sys/page_table.h>
#include <sys/phy_mem.h>

struct pcb foo_p;
struct pcb bar_p;
//struct pcb *f1;
//struct pcb *f2;
struct pcb *current[2];
int flag = 0;
void foo();
void bar();
uint64_t kernmem; 
void *km, *pf, *pb;

void _ptcr3(uint64_t ); //setting cr3 register to kick start paging
void alloc_page_dir(struct pcb* p )
{
  
  printf("Before");	 
  p->cr3 = (uint64_t)set_task_paging();
  printf("After");	 
  return ;

}

//void _asm_context(uint64_t);
void call_first(void * kmem, void * pfree, void * pbase)
{
  km = kmem;
  pf = pfree;
  pb=pbase;
  clrscr();
	bar_p.rsp_p = (uint64_t)&(bar_p.k_stack[51]);
	bar_p.k_stack[63] = (uint64_t)&bar;
	foo_p.rsp_p = (uint64_t)&(foo_p.k_stack[63]);
	foo_p.k_stack[63] = (uint64_t)&foo;
	current[0] = &(foo_p);
	current[1] = &(bar_p);
	printf("\n Inside call");
//	_asm_context((foo_p.rsp_p));
  alloc_page_dir(&(bar_p));
//  alloc_page_dir(&(foo_p));
//  while(1);
	__asm__(
		"movq %0, %%rsp;"
		:
		:"r"((foo_p.rsp_p))
	);	
	__asm__(
		"retq;"
	);
}

void schedule()
{
	struct pcb *pt1, *pt2;

	if (flag)
	{
		pt1 = (current[1]);
		pt2 = (current[0]);
	}
	else
	{	
		pt1 = current[0];
		pt2 = (current[1]);
	}	
	// Should save state of caller here 

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
		:"=g"(pt1->rsp_p)
		:
	  :"memory"
	);

	/* Now change the %rsp to callee rsp */
/*	__asm volatile(
		"movq %0, %%cr3;"
		:
		:"r"((pt2->cr3))
    :"memory"
	);*/
	/* Now change the %rsp to callee rsp */
	__asm volatile(
		"movq %0, %%rsp;"
		:
		:"r"((pt2->rsp_p))
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


	__asm volatile(
		"retq;"
	);	
}

void foo()
{
	flag = 0;
	printf("\n In foo");
	int i = 0;
	while (i < 5)
	{
		i++;
		printf(" Hello: %d\n", i);
		schedule();
		flag = 0;
	}
	while(1);
}

void bar()
{
	flag = 1;
	int i = 20;
	printf("\n In bar");
	while(i < 25)
	{
		i++;
		printf("World: %d\n",i );
		schedule();
		flag = 1;
	}

	while(1);
}

