#include <defs.h>
#include <stdio.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/isr.h>
#include <sys/irq.h>

void start(void* modulep, void* physbase, void* physfree)
{
  int a, b,c;
  a=5;
  b=0;
  c = 9;
	// kernel starts here
  clrscr();
  printf("Checking printf before ISR()\n");
//  c= a/b;
  printf("Checking printf after ISR() %d %d %d\n", a,b,c);
  printf("Checking printf\n");
  printf("Checking printf\n");
}

#define INITIAL_STACK_SIZE 4096
char stack[INITIAL_STACK_SIZE];
uint32_t* loader_stack;
extern char kernmem, physbase;

void boot(void)
{
	// note: function changes rsp, local stack variables can't be practically used
	//register char *temp1, *temp2;
	__asm__(
		"movq %%rsp, %0;"
		"movq %1, %%rsp;"
		:"=g"(loader_stack)
		:"r"(&stack[INITIAL_STACK_SIZE])
	);
	reload_gdt();
	reload_idt();
	start(
		(char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase,
		&physbase,
		(void*)(uint64_t)loader_stack[4]
	);
	/*for(
		temp1 = "!!!!! start() returned !!!!!", temp2 = (char*)0xb8000;
		*temp1;
		temp1 += 1, temp2 += 2
	) *temp2 = *temp1;*/
	while(1);
}
