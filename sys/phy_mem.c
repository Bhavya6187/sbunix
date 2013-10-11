#include <defs.h>
#include <stdio.h>
#include <sys/phy_mem.h>

#define PAGE_SIZE 4096

//static uint64_t phy_addr_map;
free_list *head_fl = NULL;

// Setting up the available memory into free pages
void phy_mem_init(uint64_t sbase, uint64_t slength, void *kernmem) 
{
  uint64_t *physfree = (uint64_t *)kernmem;
  free_list *newp = NULL;
  uint64_t page;
  if(!head_fl) //head in free list is NULL
  {
    head_fl = (free_list *)physfree;
    printf("pf=%x h=%x hn=%x \n",physfree,head_fl, head_fl->next);
    head_fl->next = NULL;
    head_fl->addr = NULL;  //Dummy node
  }
  for( page=sbase ; page < (sbase + slength) ; page+=PAGE_SIZE )
  {
    physfree +=2;
    newp = (free_list *)physfree;
    newp->addr = page;
    newp->next = head_fl;
    head_fl = newp;
    printf("pf=%x h=%x hn=%x ha=%x pg=%x b=%x l=%x\n",physfree,head_fl, head_fl->next, head_fl->addr, page, sbase, slength);
  }

}



