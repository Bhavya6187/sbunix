#include <defs.h>
#include <stdio.h>
#include <sys/phy_mem.h>

#define PAGE_SIZE 4096

//static uint64_t phy_addr_map;
free_list *head_fl = NULL;
uint64_t i=0;
// Setting up the available memory into free pages
void phy_mem_init(uint64_t sbase, uint64_t slength, void *pf, uint64_t physbase) 
{
  uint64_t *physfree = (uint64_t *)pf;
  uint64_t *pff = (uint64_t *)pf;
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
    if(page>=physbase && page<=(uint64_t)pff+(1024*1024))
    {
      //printf("p=%x pf=%x pb=%x", page, physfree, physbase);
      //page = page + (uint64_t)physfree - physbase; 
      continue;
    }
    physfree +=2;
    newp = (free_list *)physfree;
    newp->addr = page;
    newp->next = head_fl;
    head_fl = newp;
    i++;
    //printf("pf=%x h=%x hn=%x ha=%x pg=%x b=%x l=%x\n",physfree,head_fl, head_fl->next, head_fl->addr, page, sbase, slength);
  }
    printf("pf=%x h=%x hn=%x ha=%x pg=%x b=%x l=%x\n",physfree,head_fl, head_fl->next, head_fl->addr, page, sbase, slength);
    printf("No of Free Pages = %d\n", i);
    printf("Location of Head = %x\n", head_fl);

}

uint64_t allocate_free_phy_page()
{
  free_list *temp = NULL;
  temp = head_fl;
  head_fl = head_fl->next;
  return temp->addr;
}

void free_phy_page(uint64_t temp_addr)
{
  free_list *temp = NULL;
  temp->addr = temp_addr;
  temp->next = head_fl;
  head_fl = temp;
}
