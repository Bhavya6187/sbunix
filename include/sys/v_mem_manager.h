#ifndef _V_MEM_MANAGER
#define _V_MEM_MANAGER

uint64_t opfree;
extern uint64_t cur_VK;		// Current free Virtual Memory above Physfree;
void *k_malloc(uint64_t no);	// VM allocator

extern uint64_t cur_PK;		// Current free Virtual Memory below kernmem + Physfree
void *p_malloc(uint64_t no);	// VM allocator for Process space brk() 

/* maps elf binaries segments into Virtual Memory*/
uint32_t m_map(uint64_t start, uint64_t source, uint64_t f_size, uint64_t m_size);

/* creates and map Process page table (only PML4E) */
uint64_t map_pageTable();

/* Creates Process Stack */
uint64_t *process_stack();	

void test();
#endif
