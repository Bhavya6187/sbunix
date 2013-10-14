#include <defs.h>
#include <stdio.h>
#include <sys/phy_mem.h>
#include <sys/page_table.h>

#define PAGE_SIZE 4096
void _ptcr0(uint64_t ); //set 32nd bit and enables paging
void _ptcr3(uint64_t ); //setting cr3 register to kick start paging

// Setting Virtual Page Tables
void set_paging(void * km, void * pf, void * pb)
{
  uint64_t kernmem, physfree, physbase;
  uint64_t *pml4e, *pdpe, *pde, *pte, *pte_b;
  uint64_t vmem1, vmem2, vmem3, vmem4;
  uint64_t cr3;
  kernmem = (uint64_t)km;
  physfree = (uint64_t)pf;
  physbase = (uint64_t)pb;
  pml4e = (uint64_t *)allocate_free_phy_page(); 
  pdpe = (uint64_t *)allocate_free_phy_page();
  pde = (uint64_t *)allocate_free_phy_page();
  pte = (uint64_t *)allocate_free_phy_page();

  vmem1 = ((kernmem << 16) >> 55); 
  vmem2 = ((kernmem << (16+9)) >> 55); 
  vmem3 = ((kernmem << (16+9+9)) >> 55); 
  vmem4 = ((kernmem << (16+9+9+9)) >> 55); 
  printf("v1=%d v2=%d v3=%d v4=%d", vmem1, vmem2, vmem3, vmem4);

  //Creating Page Table heirarchy - Page Table Translation
  //Mapping is done as per AMD manual PML4E, PDPE, PDE structure
  //First 12 bits are 0, Next 40 bits are address bits, last 3 bits are set to 011
  //pml4e[vmem1] = ((uint64_t)pdpe >> 12) | 3;
  //pdpe[vmem2] = ((uint64_t)pde >> 12) | 3;
  //pde[vmem3] = ((uint64_t)pte >> 12) | 3;
  pml4e[vmem1] = ((uint64_t)pdpe >> 12) ;
  pdpe[vmem2] = ((uint64_t)pde >> 12) ;
  pde[vmem3] = ((uint64_t)pte >> 12);

  int i,j;
  for(i=0, j=0; i<physfree-physbase; i+=PAGE_SIZE,j++ )
  {
    pte_b = (uint64_t *)allocate_free_phy_page();
    //pte[vmem4+j] = (((uint64_t)pte_b >> 12) | 3);
    pte[vmem4+j] = (((uint64_t)pte_b >> 12));
    //break;
  }

  //cr3 = ((uint64_t)pml4e >> 12);
  cr3 = ((uint64_t)pml4e);
  // Setting the Page Tables
  //_ptcr0((uint64_t)0x9000000000000000); //set 32nd bit and enables paging
  _ptcr3(cr3); //setting cr3 register to kick start paging


  //cr3 = ((uint64_t)pml4e >> 12) | 3;
}

