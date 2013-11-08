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
	bar_p.rsp_p = (uint64_t)&(bar_p.k_stack);
	bar_p.k_stack[0] = (uint64_t)&bar;
	foo_p.rsp_p = (uint64_t)&(foo_p.k_stack);
	foo_p.k_stack[0] = (uint64_t)&foo;
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

void schedule(struct pcb pt1, struct pcb pt2)
{
	// Should save state of caller here 
	__asm__(
		"movq %%rsp, %0"
		:"=g"(pt1.rsp_p)
		:
	);

	/* Now change the %rsp to callee rsp */
	__asm__(
		"movq %0, %%rsp;"
		:
		:"r"((pt2.rsp_p))
		
	);
	__asm__(
		"retq;"
	);	
}

void foo()
{
//	printf("\n In foo");

//	printf("\n Hello:");
	while (1)
	{
//		schedule(foo_p, bar_p);
//	}
 __asm__(
		"movq %0, %%rsp;"
		:
		:"r"((bar_p.rsp_p))
	);
	 __asm__(
	 	"retq;"
        );      
  }
}

void bar()
{

//	printf("\n World:");
	while(1)
	{
    __asm__(
		"movq %0, %%rsp;"
		:
		:"r"((foo_p.rsp_p))
	  );
	  __asm__(
	 	"retq;"
    );      
//		schedule(bar_p, foo_p);
	}
}

