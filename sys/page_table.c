#include <defs.h>
#include <stdio.h>
#include <sys/phy_mem.h>
#include <sys/page_table.h>

#define PAGE_SIZE 4096
void _ptcr0(uint64_t ); //set 32nd bit and enables paging
void _ptcr3(uint64_t ); //setting cr3 register to kick start paging


int getPTEindex(uint64_t vadd)
{
  return ((vadd << (16+9+9+9)) >> 55); 
}

int getPDEindex(uint64_t vadd)
{
  return ((vadd << (16+9+9)) >> 55); 
}

int getPDPEindex(uint64_t vadd)
{
  return ((vadd << (16+9)) >> 55); 
}

int getPML4Eindex(uint64_t vadd)
{
  return ((vadd << 16) >> 55); 
}

// Setting Virtual Page Tables
void set_paging(void * km, void * pf, void * pb)
{
  uint64_t kernmem, physfree, physbase;
  uint64_t *pml4e, *pdpe, *pde, *pte;
  int vmem1, vmem2, vmem3, vmem4;
  uint64_t cr3;
  kernmem = (uint64_t)km;
  physfree = (uint64_t)pf;
  physbase = (uint64_t)pb;
  
  printf("%p\n",kernmem);

  pml4e = (uint64_t *)allocate_free_phy_page(); 
  pdpe = (uint64_t *)allocate_free_phy_page();
  pde = (uint64_t *)allocate_free_phy_page();
  pte = (uint64_t *)allocate_free_phy_page();
 
  /*
  vmem1 = ((kernmem << 16) >> 55); 
  vmem2 = ((kernmem << (16+9)) >> 55); 
  vmem3 = ((kernmem << (16+9+9)) >> 55); 
  vmem4 = ((kernmem << (16+9+9+9)) >> 55); 
  */
  vmem1 = getPML4Eindex(kernmem);
  vmem2 = getPDPEindex(kernmem);
  vmem3 = getPDEindex(kernmem);
  vmem4 = getPTEindex(kernmem);
  //printf("v1=%d v2=%d v3=%d v4=%d \n", vmem1, vmem2, vmem3, vmem4);

  //Creating Page Table heirarchy - Page Table Translation
  //Mapping is done as per AMD manual PML4E, PDPE, PDE structure
  //First 12 bits are 0, Next 40 bits are address bits, last 3 bits are set to 011
  pml4e[vmem1] = (((uint64_t)pdpe) & 0xFFFFFFFFFF000) | 3;
  pdpe[vmem2]  = (((uint64_t)pde)  & 0xFFFFFFFFFF000) | 3;
  pde[vmem3]   = (((uint64_t)pte)  & 0xFFFFFFFFFF000) | 3;
  pml4e[510] = (((uint64_t)pml4e)  & 0xFFFFFFFFFF000) | 3;
  //pml4e[vmem4] = (((uint64_t)pml4e)  & 0xFFFFFFFFFF000) | 3;

  uint64_t i; 
  int j;
  for(j=0,i=physbase; i<physfree; i+=PAGE_SIZE,j++ )
  {
    pte[vmem4+j] = ((((uint64_t)i) & 0xFFFFFFFFFF000) | 3);
  }
  //printf("\n %d %d %d %d\n", vmem1, vmem2, vmem3, vmem4);
   
  /*pdpe_vm = (uint64_t *)allocate_free_phy_page();
  pde_vm = (uint64_t *)allocate_free_phy_page();
  pml4e[vmem1] = (((uint64_t)pdpe_vm) & 0xFFFFFFFFFF000) | 3;
  pdpe[vmem2]  = (((uint64_t)pde_vm)  & 0xFFFFFFFFFF000) | 3;
  pde[vmem3]   = (((uint64_t)pte_vm)  & 0xFFFFFFFFFF000) | 3;*/
  //pte_vm[vmem4] = ((((uint64_t)0xB8000000) & 0xFFFFFFFFFF000) | 3);
  //for(j=0, i=0xB8000; j<=0; i+=PAGE_SIZE,j++ )
    //pte_vm[vmem4+j] = ((((uint64_t)i) & 0xFFFFFFFFFF000) | 3);
  //pte_vm = (uint64_t *)allocate_free_phy_page();
  //pte[vmem4] = ((((uint64_t)0xB8000) & 0xFFFFFFFFFF000) | 3);
  pte[vmem4+j] = (0xB8000  | 7);
  //printf("\n %d %d %d %d %p %p %p\n", vmem1, vmem2, vmem3, vmem4, pml4e[vmem1], pte_vm[vmem4], video_mem);
  //printf("j=%d\n",j);
  //printf("%p %p %p %p %p %p %p %p\n", pml4e, pdpe, pde, pte, pdpe_vm, pde_vm, pte_vm);

  cr3 = ((uint64_t)pml4e);
  // Setting the Page Tables
  _ptcr3(cr3); //setting cr3 register to kick start paging
  
  // Mapping video memory
  video_vm = (0xffffffff80000000|(uint64_t)physfree );

}

//0xFFFF000000000000 | 0x0000FF7FBFDFE 
void page_mapping(uint64_t vadd)
{
    int pml4eindex = getPML4Eindex(vadd);
    int pdpeindex = getPDPEindex(vadd);
    int pdeindex = getPDEindex(vadd);
    int pteindex = getPTEindex(vadd);
    
    uint64_t *pml4e, *pdpe, *pde, *pte, *paddr;
    //Read cr3 here -
  	/*asm volatile(
		"movq %%cr3, %0"
		:"=g"(pml4e)
		:
	  :"memory"
	  );*/
    uint64_t base;
    //, setindices1, setindices2, setindices3, setindices4;
    printf("In page_mapping\n");
    // Access PML4E table
    base = 0xFFFF000000000000; 
    base = (((base >> (12+9+9+9+9))<<9 | 0x1FE ) << (12+9+9+9) );
    base = (((base >> (12+9+9+9))<<9 | 0x1FE   ) << (12+9+9) );
    base = (((base >> (12+9+9))<<9 | 0x1FE     ) << (12+9) );
    base = (((base >> (12+9))<<9 | 0x1FE       ) << (12) );
    pml4e = (uint64_t*)base;
    pdpe = (uint64_t*)pml4e[pml4eindex];
    if(!pdpe)
    {
        pdpe = (uint64_t *)allocate_free_phy_page();
        pml4e[pml4eindex] = (((uint64_t)pdpe) & 0xFFFFFFFFFF000) | 3;
    }
    
    // Access PDPE table
    base = 0xFFFF000000000000; 
    base = (((base >> (12+9+9+9+9))<<9 | 0x1FE ) << (12+9+9+9) );
    base = (((base >> (12+9+9+9))<<9 | 0x1FE   ) << (12+9+9) );
    base = (((base >> (12+9+9))<<9 | 0x1FE     ) << (12+9) );
    base = (((base >> (12+9))<<9 |pml4eindex   ) << (12) );
    pdpe = (uint64_t*)base;
    pde = (uint64_t*)pdpe[pdpeindex];
    if(!pde)
    {
        pde = (uint64_t *)allocate_free_phy_page();
        pdpe[pdpeindex] = (((uint64_t)pde) & 0xFFFFFFFFFF000) | 3;
    }

    // Access PDE table
    base = 0xFFFF000000000000; 
    base = (((base >> (12+9+9+9+9))<<9 | 0x1FE ) << (12+9+9+9) );
    base = (((base >> (12+9+9+9))<<9 | 0x1FE   ) << (12+9+9) );
    base = (((base >> (12+9+9))<<9 | pml4eindex) << (12+9) );
    base = (((base >> (12+9))<<9 |pdpeindex    ) << (12) );
    pde = (uint64_t*)base;
    pte = (uint64_t*)pde[pdeindex];
    if(!pte)
    {
        pte = (uint64_t *)allocate_free_phy_page();
        pde[pdeindex] = (((uint64_t)pte) & 0xFFFFFFFFFF000) | 3;
    }

    // Access PTE table
    base = 0xFFFF000000000000; 
    base = (((base >> (12+9+9+9+9))<<9 | 0x1FE   ) << (12+9+9+9) );
    base = (((base >> (12+9+9+9))<<9 | pml4eindex) << (12+9+9) );
    base = (((base >> (12+9+9))<<9 | pdpeindex   ) << (12+9) );
    base = (((base >> (12+9))<<9 |pdeindex       ) << (12) );
    pte = (uint64_t*)base;
    paddr = (uint64_t*)pte[pteindex];
    if(!paddr)
    {
        paddr = (uint64_t *)allocate_free_phy_page();
        pte[pteindex] = (((uint64_t)paddr) & 0xFFFFFFFFFF000) | 3;
    }
    printf("In page_mapping\n");

}

