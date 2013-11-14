#ifndef _PHY_MEM_H
#define _PHY_MEM_H

typedef struct free_lst {
  uint64_t addr; 
  struct free_lst *next;
}free_list; 

struct freelist {
  uint64_t addr; 
  int flag; // flag=0 or 1 i if page is free
}; 

struct freelist fl[40000]; 


extern free_list *head_fl;
extern uint64_t vm_fl;
struct smap_t {
	uint64_t base, length;
	uint32_t type;
}__attribute__((packed)) *smap;

void phy_mem_init(uint64_t sbase, uint64_t slength, void* physfree, uint64_t physbase);
uint64_t allocate_free_phy_page2();
void free_phy_page(uint64_t temp_addr);

void pm_init(uint64_t sbase, uint64_t slength, void *pf, uint64_t physbase); 
uint64_t allocate_free_phy_page();

#endif
