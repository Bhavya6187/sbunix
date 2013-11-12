#include <defs.h>
#include <stdio.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/isr.h>
#include <sys/irq.h>
#include <sys/phy_mem.h>
#include <sys/page_table.h>
#include <sys/tasking.h>
#include <sys/tarfs.h>



#define INITIAL_STACK_SIZE 4096
char stack[INITIAL_STACK_SIZE];
uint32_t* loader_stack;
extern char kernmem, physbase;
struct tss_t tss;

void start(uint32_t* modulep, void* physbase, void* physfree)
{
  clrscr();
	struct smap_t {
		uint64_t base, length;
		uint32_t type;
	}__attribute__((packed)) *smap;
	while(modulep[0] != 0x9001) modulep += modulep[1]+2;
	for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
		if (smap->type == 1 /* memory */ && smap->length != 0) {
			printf("Available Physical Memory [%x-%x]\n", smap->base, smap->base + smap->length);
      phy_mem_init(smap->base, smap->length, physfree, (uint64_t)(physbase));
      printf("physfree %x\n", (uint64_t)physfree);
		}
	}

  physfree = physfree + (1024*1024);

  set_paging((void *)&kernmem, physfree, physbase);
  putint(100);
  set_paging((void *)&kernmem, physfree, physbase);
  
  //char* x = (char*)0xFFFF8000000B8000;
  /*volatile char *x = (char*)(0xffffffff80000000|(uint64_t)physfree );
  *x++ = 'A';
  *x++ = 0x1F; */
  //set_virtual_video_memory((void*)0xFFFFFFFF000B8000);
  //clrscr();
  //while(infinite_loop);
  
  //page_mapping((uint64_t)0xFFFFFFFF80200000);
  page_mapping((uint64_t)0xFFFFFF00802FF000);
  //page_mapping((uint64_t)0xFFFFFFFF00000000);
  printf("%d \n", 5454);
  /*page_mapping((uint64_t)0x00000000000B8000);
  char* x1 = (char*)0x00000000000B8000;
  *x1 = 'C';
  putchar(*x1);
  */
  /*
  int a, b,c ;
  b =0;
  a = 5;
  c = a/b;
  putint(c);
  */
  putchar('D');
  puts("hey");

	printf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);
	read_tarfs();
	while(1);
}

void boot(void)
{
	// note: function changes rsp, local stack variables can't be practically used
	register char *temp1, *temp2;
	__asm__(
		"movq %%rsp, %0;"
		"movq %1, %%rsp;"
		:"=g"(loader_stack)
		:"r"(&stack[INITIAL_STACK_SIZE])
	);
	reload_gdt();
	setup_tss();
	start(
		(uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
		&physbase,
		(void*)(uint64_t)loader_stack[4]
	);
	for(
		temp1 = "!!!!! start() returned !!!!!", temp2 = (char*)0xb8000;
		*temp1;
		temp1 += 1, temp2 += 2
	) *temp2 = *temp1;
	while(1);
}
