/* Implements process management  */

#include <stdio.h>
#include <defs.h>
#include <sys/tasking.h>
#include <sys/page_table.h>
#include <sys/phy_mem.h>
#include <sys/gdt.h>

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
  
  p->cr3 = (uint64_t)set_task_paging();
  return ;

}

void alloc_user_stack(struct pcb* p)
{
  p->u_stack = (uint64_t*)0x0000000000800000;
  page_mapping((uint64_t)p->u_stack);
}

void switch_to_user_mode()
{

  alloc_page_dir(&(bar_p));
  _ptcr3(bar_p.cr3);
  alloc_user_stack(&(bar_p));
	bar_p.rsp_p = (uint64_t)&(bar_p.k_stack[63]);
	printf("\n Inside call\n");
  tss.rsp0 = bar_p.rsp_p;

  alloc_page_dir(&(bar_p));
   uint64_t tem = 0x28; 
  __asm volatile("mov %0,%%rax;"::"r"(tem));
  __asm volatile("ltr %ax");
  __asm volatile("\
       push $0x23;\
       push %0;\
       pushf;\
       push $0x1B;\
       push %1"::"g"(&bar_p.u_stack[511]),"g"(&bar):"memory");
  __asm volatile("\
     iretq;\
    ");
}


void call_first(void * kmem, void * pfree, void * pbase)
{
  km = kmem;
  pf = pfree;
  pb=pbase;
  //clrscr();
	bar_p.rsp_p = (uint64_t)&(bar_p.k_stack[51]);
	bar_p.k_stack[63] = (uint64_t)&bar;
	foo_p.rsp_p = (uint64_t)&(foo_p.k_stack[63]);
	foo_p.k_stack[63] = (uint64_t)&foo;
	current[0] = &(foo_p);
	current[1] = &(bar_p);
	printf("\n Inside call\n");
  alloc_page_dir(&(foo_p));
  alloc_page_dir(&(bar_p));
  _ptcr3(foo_p.cr3);
 
  printf("\nfoo cr3- %x\n", foo_p.cr3);
  printf("bar cr3- %x\n", bar_p.cr3);
	/*__asm volatile(
		"movq %0, %%cr3;"
		:
		:"r"((foo_p.cr3))
    :"memory"
	);*/
  //while(1);
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
	__asm volatile(
		"movq %0, %%cr3;"
		:
		:"r"((pt2->cr3))
    :"memory"
	);
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
	char *a=(char*)video_vm;
  *a = 'a';
  __asm__ volatile("int $80;");
  *a='x';
  //clrscr();
  //printf("Bhavya");


	while(1);
}

