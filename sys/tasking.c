/* Implements process management  */

# include <stdio.h>
# include <defs.h>
# include <sys/tasking.h>
#include <sys/page_table.h>

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

void alloc_page_dir(struct pcb* p )
{
  
  //p->cr3 = set_paging(km, pf, pb);

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
  //alloc_page_dir(&(foo_p));
	current[0] = &(foo_p);
	current[1] = &(bar_p);
	printf("\n Inside call");
//	_asm_context((foo_p.rsp_p));
	 
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

	asm volatile(
		"push %rax"
		);
	asm volatile(
		"push %rbx"
		);
	asm volatile(
		"push %rcx"
		);
	asm volatile(
		"push %rdx"
		);

	asm volatile(
		"push %rbp"
		);
	asm volatile(
		"push %r8"
		);
	asm volatile(
		"push %r9"
		);
	asm volatile(
		"push %r10"
		);
	asm volatile(
		"push %r11"
		);
	asm volatile(
		"push %r12"
		);
	asm volatile(
		"push %r13"
		);
	asm volatile(
		"push %r14"
		);
	asm volatile(
		"movq %%rsp, %0"
		:"=g"(pt1->rsp_p)
		:
	        :"memory"
	);

	/* Now change the %rsp to callee rsp */
	asm volatile(
		"movq %0, %%rsp;"
		:
		:"r"((pt2->rsp_p))
	        :"memory"
	);
	asm volatile(
		"pop %r14"
		);
	asm volatile(
		"pop %r13"
		);
	asm volatile(
		"pop %r12"
		);
	asm volatile(
		"pop %r11"
		);

	asm volatile(
		"pop %r10"
		);
	asm volatile(
		"pop %r9"
		);
	asm volatile(
		"pop %r8"
		);
	asm volatile(
		"pop %rbp"
		);
	asm volatile(
		"pop %rdx"
		);
	asm volatile(
		"pop %rcx"
		);
	asm volatile(
		"pop %rbx"
		);
	asm volatile(
		"pop %rax"
		);


	asm volatile(
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

