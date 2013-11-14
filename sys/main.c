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
   // volatile char *p = (volatile char*)(0x40000);
   //volatile char *p = (volatile char*)(0xB8000);
    // *(p) = 67;
	  //Read cr3 here -
  uint64_t a;
  __asm volatile(
	"movq %%cr3, %0"
	:"=g"(a)
	:
	:"memory"
	);
  printf("cr3 = %p\n", a);
  char *d;
  d = (char*)a;
  printf("cr3 = %d\n", *d);

  struct smap_t {
		uint64_t base, length;
		uint32_t type;
	}__attribute__((packed)) *smap;
	while(modulep[0] != 0x9001) modulep += modulep[1]+2;
	for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
		if (smap->type == 1 /* memory */ && smap->length != 0) {
			printf("Available Physical Memory [%p-%p]\n", smap->base, smap->base + smap->length);
      //phy_mem_init(smap->base, smap->length, physfree, (uint64_t)(physbase));
      pm_init(smap->base, smap->length, physfree, (uint64_t)(physbase));
      printf("physfree %p\n", (uint64_t)physfree);
		}
	}
  physfree = physfree + (1024*1024);
      printf("physfree %p\n", (uint64_t)physfree);

  int j;
  for(j=0; j<31000; j++)
  {
    //printf("fl in allocate : %d %p %d\n", j, fl[j].addr,fl[j].flag);
    if(fl[j].flag==1)
    {
      printf("fl in allocate : %d %p %d\n", j, fl[j].addr,fl[j].flag);
      break;
    }
  }


  uint64_t *test;
  int i=0;
  for(i=0;i<1;i++)
  {
   test = (uint64_t*)allocate_free_phy_page();
   printf("%d %p\n",i, test);
  }

  printf("kernmem %p\n", (uint64_t)kernmem);
  set_paging((void *)&kernmem, physfree, physbase);
  putint(100);
  
  //char* x = (char*)0xFFFF8000000B8000;
  /*volatile char *x = (char*)(0xffffffff80000000|(uint64_t)physfree );
  *x++ = 'A';
  *x++ = 0x1F; */
  //set_virtual_video_memory((void*)0xFFFFFFFF000B8000);
  //clrscr();
  //while(infinite_loop);
  /*page_mapping((uint64_t)0xFFFFFFFF80200000);
  page_mapping((uint64_t)0xFFFFFF00802FF000);
  page_mapping((uint64_t)0xFFFFEE00802FF000);
  page_mapping((uint64_t)0xFFFFFF00802FF000);
  */
  /*intf("%d \n", 5454);
  for(i=0;i<4;i++)
  {
   test = (uint64_t*)allocate_free_phy_page();
   printf("%d %x \n", i, test);
  }*/


  //uint64_t *test1 = (uint64_t*)0xFFFF00FF80000000;
  //uint64_t *test1 = (uint64_t*)0xFFFFFF00802FF000;
  //uint64_t *test1 = (uint64_t*)0xFFFFFFFF80000000;
  //page_mapping((uint64_t)0xFFFFFFFF00000000);
  /*printf("Checking mapping\n");
  page_mapping((uint64_t)test1);

  printf("test = %x", *test1);
  printf("Checking mapping again\n");
  page_mapping((uint64_t)0xFFFFFFFF80000000);

  *test1 = 5;
  printf("test = %x", *test1);
*/
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
  

  call_first(kernmem, physfree, physbase);
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
  reload_idt();
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
