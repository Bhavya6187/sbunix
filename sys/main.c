#include <defs.h>
#include <stdio.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/isr.h>
#include <sys/irq.h>
#include <sys/phy_mem.h>
#include <sys/page_table.h>


#define INITIAL_STACK_SIZE 4096
char stack[INITIAL_STACK_SIZE];
uint32_t* loader_stack;
extern char kernmem, physbase;

void start(uint32_t* modulep, void* physbase, void* physfree)
{
  struct smap_t *smap;
  clrscr();
	printf("physbase-%x\n physfree %x\n", (uint64_t)physbase, (uint64_t)physfree);
	
  //printf("modeulep = %x %x %x\n", (uint64_t)modulep[0], (uint64_t)modulep[1], (uint64_t)&modulep);
	while(modulep[0] != 0x9001) modulep += modulep[1]+2;
	for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
		if (smap->type == 1 /* memory */ && smap->length != 0) {
			printf("Available Physical Memory [%x-%x]\n", smap->base, smap->base + smap->length);
      phy_mem_init(smap->base, smap->length, physfree, (uint64_t)(physbase));
	    printf("physfree %x\n", (uint64_t)physfree);
		}
	}
   
  printf("old physfree = %x", physfree);
  //physfree = physfree + (1024*1024);
  printf("New physfree = %x", physfree);
  
  /*
  uint64_t t1, t2,t3,t4;
  t1= allocate_free_phy_page();
  t2= allocate_free_phy_page();
  t3= allocate_free_phy_page();
  printf("t1=%x t2=%x t3=%x\n", t1, t2, t3);
  free_phy_page(t2);
  t1= allocate_free_phy_page();
  t2= allocate_free_phy_page();
  t4= allocate_free_phy_page();
  printf("t1=%x t2=%x t4=%x\n", t1, t2, t4);
  */
  
  uint64_t a;
  a = (uint64_t)&kernmem;
  printf("kernmem %ld ", a);
  printf("kernmem %p \n", a);
  printf("pf = %p pb = %p \n", physfree, physbase);
  set_paging((void *)&kernmem, physfree, physbase);
  //printf("REturn from Paging :P\n");
	// kernel starts here
	//while(1);
}

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
	setup_tss();
	//reload_idt();
	start(
		(uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
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
