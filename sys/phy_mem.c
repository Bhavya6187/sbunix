#include <defs.h>
#include <stdio.h>
#include <sys/phy_mem.h>
#include <sys/page_table.h>

#define PAGE_SIZE 4096
#define N 32768

//static uint64_t phy_addr_map;
free_list *head_fl = NULL;
uint64_t i=0;
uint64_t vm_fl = 0;

uint64_t set_task_paging()
{
    uint64_t pml4e = (uint64_t )allocate_free_phy_page();
    uint64_t base1 = 0xFFFF000000000000; 
    uint64_t* temp_pm;
    base1 = 0xFFFFFF7FBFDFE000; // 510 510 510 510 
    temp_pm = (uint64_t*)base1;

    if(temp_pm[509] == 0x0)
    {
      temp_pm[509] =(((uint64_t)pml4e)  & 0xFFFFFFFFFF000) | 7;// (uint64_t)(pml4e);
      uint64_t base2 = 0xFFFFFF7FBFDFD000;//  510 510 510 509 000 
      uint64_t* page_pm =(uint64_t *) base2;
      //page_pm[511] = temp_pm[511];
      page_pm[510] = (((uint64_t)temp_pm[509])  & 0xFFFFFFFFFF000) | 7;
      page_pm[511] = (((uint64_t)temp_pm[511])  & 0xFFFFFFFFFF000) | 7;
      printf("returning pml4e to be - %x\n",pml4e);
      return pml4e;
    }

    if(temp_pm[508] == 0x0)
    {
      temp_pm[508] =(((uint64_t)pml4e)  & 0xFFFFFFFFFF000) | 7;// (uint64_t)(pml4e);
      uint64_t base2 = 0xFFFFFF7FBFDFC000;//  510 510 510 508 000 
      uint64_t* page_pm =(uint64_t *) base2;
      page_pm[511] = temp_pm[511];
      page_pm[510] = temp_pm[508];
      printf("returning pml4e to be - %x\n",pml4e);
      return pml4e;
    }
    return pml4e;
}

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
    head_fl->next = NULL;
    head_fl->addr = NULL;  //Dummy node
  }
  for( page=sbase ; page < (sbase + slength) ; page+=PAGE_SIZE )
  {
    if(page>=physbase && page<=(uint64_t)pff+(1024*1024))
    {
      continue;
    }
    physfree +=2;
    newp = (free_list *)physfree;
    newp->addr = page;
    newp->next = head_fl;
    head_fl = newp;
    i++;
  }
}

uint64_t allocate_free_phy_page2()
{
  free_list *temp = NULL;
  temp = (free_list*)((uint64_t)head_fl | vm_fl);
//  temp = head_fl;
  head_fl = temp->next;
  return temp->addr;
}

void free_phy_page2(uint64_t temp_addr)
{
  free_list *temp = NULL;
  temp->addr = temp_addr;
  temp->next = head_fl;
  head_fl = temp;
}


void pm_init(uint64_t sbase, uint64_t slength, void *pf, uint64_t physbase) 
{
  uint64_t *pff = (uint64_t *)pf;
  uint64_t page;
  int index;
  printf("sbase - %p slength - %p \n", sbase, sbase+slength);
  for( page=sbase ; page < (sbase + slength) ; page+=PAGE_SIZE )
  {
    //if(page>=physbase && page<=(uint64_t)pff+(1024*1024))
    if(page<(uint64_t)pff+(1024*1024))
    {
      //printf("p=%x pf=%x pb=%x", page, physfree, physbase);
      index = (int)(page/PAGE_SIZE);
      fl[index].addr = page; 
      fl[index].flag = 0; 
      //printf("Free Page at %p %d %p %d\n", page, index, fl[index].addr, fl[index].flag); 
    }
    else
    {
      index = (int)(page/PAGE_SIZE);
      fl[index].addr = page; 
      fl[index].flag = 1;
      // Zero out pages !! :( Not working
      uint64_t j=0;
      volatile char* p = (volatile char*)(page);
      for(j=0; j<PAGE_SIZE; j++)
        *(p+j) = 0;
      i++;
    }
    //printf("Free Page at %p %d %p %d\n", page, index, fl[index].addr, fl[index].flag); 
  }
  printf("No of Free Pages = %d\n", i);
}

uint64_t allocate_free_phy_page()
{
  uint64_t temp=0;
  int found=0;
  int i;
  for(i=0; i<N; i++)
  {
    if(fl[i].flag==1)
    {
      temp = fl[i].addr;
      fl[i].flag = 0;
      found = 1;
      break;
    }
  }
  if(found)
  {
    //printf("Page found at address=%p\n", temp);
    return temp;
  }
  else
  {
    printf("No free page found\n");
    return 0;
  }
}

uint64_t free_phy_page(uint64_t temp_addr)
{
  uint64_t temp=0;
  int found=0;
  int i;
  for(i=0; i<N; i++)
  {
    if(fl[i].addr==temp_addr)
    {
      temp = fl[i].addr;
      fl[i].flag = 1;
      found =1;
      break;
    }
  }
  if(found)
  {
    temp++;//printf("Page found and freed=%p\n", temp);
    return 1;
  }
  else
  {
    printf("Page trying to free not found\n");
    return 0;
  }
}
