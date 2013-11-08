/* Implements process management  */

# include <stdio.h>
# include <defs.h>
# include <sys/tasking.h>

struct pcb foo_p;
struct pcb bar_p;
void foo();
void bar();


//void _asm_context(uint64_t);
void call_first()
{
	bar_p.rsp_p = (uint64_t)&(bar_p.k_stack[63]);
	bar_p.k_stack[63] = (uint64_t)&bar;
	foo_p.rsp_p = (uint64_t)&(foo_p.k_stack[63]);
	foo_p.k_stack[63] = (uint64_t)&foo;
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

void schedule(uint64_t* pt1, uint64_t pt2)
{
	// Should save state of caller here 
	__asm__(
		"movq %%rsp, %0"
		:"=g"(*pt1)
		:
	);

	/* Now change the %rsp to callee rsp */
	__asm__(
		"movq %0, %%rsp;"
		:
		:"r"((pt2))
	);
	__asm__(
		"retq;"
	);	
}

void foo()
{
	printf("\n Hello:");
	while (1)
	{
    printf("HI");
		schedule(&foo_p.rsp_p, bar_p.rsp_p);
	}
}

void bar()
{
	printf("\n World:");
	while(1)
	{
    printf("BI\n");
		schedule(&bar_p.rsp_p, foo_p.rsp_p);
  }
}

