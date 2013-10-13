#ifndef _PHY_MEM_H
#define _PHY_MEM_H

typedef struct free_lst {
  uint64_t addr; 
  struct free_lst *next;
}free_list; 

struct smap_t {
	uint64_t base, length;
	uint32_t type;
}__attribute__((packed)) *smap;

void phy_mem_init(uint64_t sbase, uint64_t slength, void* physfree, uint64_t physbase);
uint64_t allocate_free_phy_page();
void free_phy_page(uint64_t temp_addr);


#endif
