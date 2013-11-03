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
  uint64_t *pml4e, *pdpe, *pde, *pte;
  uint64_t *pdpe_vm, *pde_vm, *pte_vm;
  uint64_t vmem1, vmem2, vmem3, vmem4;
  uint64_t cr3;
  kernmem = (uint64_t)km;
  physfree = (uint64_t)pf;
  physbase = (uint64_t)pb;
  
  pml4e = (uint64_t *)allocate_free_phy_page(); 
  pdpe = (uint64_t *)allocate_free_phy_page();
  pde = (uint64_t *)allocate_free_phy_page();
  pte = (uint64_t *)allocate_free_phy_page();
 
  /* 
  pml4e = (uint64_t *)physfree; 
  pdpe = (uint64_t *)(physfree + PAGE_SIZE);
  pde = (uint64_t *)(physfree + 2*PAGE_SIZE); 
  pte = (uint64_t *)(physfree + 3*PAGE_SIZE);
  */

  vmem1 = ((kernmem << 16) >> 55); 
  vmem2 = ((kernmem << (16+9)) >> 55); 
  vmem3 = ((kernmem << (16+9+9)) >> 55); 
  vmem4 = ((kernmem << (16+9+9+9)) >> 55); 
  printf("v1=%d v2=%d v3=%d v4=%d \n", vmem1, vmem2, vmem3, vmem4);

  //Creating Page Table heirarchy - Page Table Translation
  //Mapping is done as per AMD manual PML4E, PDPE, PDE structure
  //First 12 bits are 0, Next 40 bits are address bits, last 3 bits are set to 011
  pml4e[vmem1] = (((uint64_t)pdpe) & 0xFFFFFFFFFF000) | 3;
  pdpe[vmem2]  = (((uint64_t)pde)  & 0xFFFFFFFFFF000) | 3;
  pde[vmem3]   = (((uint64_t)pte)  & 0xFFFFFFFFFF000) | 3;
  
  uint64_t i; 
  int j;
  for(j=0,i=physbase; i<=physfree; i+=PAGE_SIZE,j++ )
  {
    //pte_b = (uint64_t *)allocate_free_phy_page();
    //pte[vmem4+j] = ((((uint64_t)pte_b) & 0xFFFFFFFFFF000) | 3);
    pte[vmem4+j] = ((((uint64_t)i) & 0xFFFFFFFFFF000) | 3);
    //pte[vmem4+j] =  i;
    //printf("%p %p \n",(uint64_t)kernmem + i, pte[vmem4+j] );
    //break;
  }

  printf("pml4e %p",pml4e[vmem1]);
  // Setting virtual Video memory here itself 
  // set_virtual_video_memory((void*)0xFFFFFFFF000B8000);
  /*0x324000 kernmem 0xFFFFFFFF80200000
                                 324000
                               80524000*/
  uint64_t video_mem;
  //video_mem = 0xFFFFFFFF80800000;
  //video_mem = 0x0000000000000000;
  //video_mem = 0x00000000000B8000;
  video_mem = 0xB8000;
  //video_mem = 0x0000000000000000;
  vmem1 = ((video_mem << (16)) >> 55); 
  vmem2 = ((video_mem << (16+9)) >> 55); 
  vmem3 = ((video_mem << (16+9+9)) >> 55); 
  vmem4 = ((video_mem << (16+9+9+9)) >> 55);
  printf("\n %d %d %d %d\n", vmem1, vmem2, vmem3, vmem4);
   
  pdpe_vm = (uint64_t *)allocate_free_phy_page();
  pde_vm = (uint64_t *)allocate_free_phy_page();
  pte_vm = (uint64_t *)allocate_free_phy_page();
  pml4e[vmem1] = (((uint64_t)pdpe_vm) & 0xFFFFFFFFFF000) | 3;
  pdpe[vmem2]  = (((uint64_t)pde_vm)  & 0xFFFFFFFFFF000) | 3;
  pde[vmem3]   = (((uint64_t)pte_vm)  & 0xFFFFFFFFFF000) | 3;
  //pte_vm[vmem4] = ((((uint64_t)0xB8000000) & 0xFFFFFFFFFF000) | 3);
  //for(j=0, i=0xB8000; j<=0; i+=PAGE_SIZE,j++ )
    //pte_vm[vmem4+j] = ((((uint64_t)i) & 0xFFFFFFFFFF000) | 3);
  pte_vm[vmem4] = ((((uint64_t)0xB8000) & 0xFFFFFFFFFF000) | 3);
  printf("\n %d %d %d %d %p %p %p\n", vmem1, vmem2, vmem3, vmem4, pml4e[vmem1], pte_vm[vmem4], video_mem);
  printf("j=%d\n",j);
  printf("%p %p %p %p %p %p %p %p\n", pml4e, pdpe, pde, pte, pdpe_vm, pde_vm, pte_vm);
  //vm3=4; 
  //vm4=0;

	//0xB8000;
  //printf("\n j%d",j);
  /*
  printf("%p \n", pte[vmem4+j] );
  printf("%p \n", pte[vmem4+j-1] );
  printf("j = %d\n",j);
  printf("cr3 = %x",((uint64_t)pml4e));
  */
  //cr3 = ((uint64_t)pml4e >> 12);
  cr3 = ((uint64_t)pml4e);
  // Setting the Page Tables
  _ptcr3(cr3); //setting cr3 register to kick start paging
  //_ptcr0(cr0); //setting cr3 register to kick start paging
}

// Setting Virtual Address for printf()
/*void set_virtual_video_memory(void* km)
{
  uint64_t *pml4e, *pdpe, *pde, *pte;
  uint64_t vmem1, vmem2, vmem3, vmem4;
  uint64_t cr3;
  uint64_t kernmem = (uint64_t)km;
  
  pml4e = (uint64_t *)allocate_free_phy_page(); 
  pdpe = (uint64_t *)allocate_free_phy_page();
  pde = (uint64_t *)allocate_free_phy_page();
  pte = (uint64_t *)allocate_free_phy_page();
 
  vmem1 = ((kernmem << 16) >> 55); 
  vmem2 = ((kernmem << (16+9)) >> 55); 
  vmem3 = ((kernmem << (16+9+9)) >> 55); 
  vmem4 = ((kernmem << (16+9+9+9)) >> 55); 
  printf("v1=%d v2=%d v3=%d v4=%d \n", vmem1, vmem2, vmem3, vmem4);

  //Creating Page Table heirarchy - Page Table Translation
  //Mapping is done as per AMD manual PML4E, PDPE, PDE structure
  //First 12 bits are 0, Next 40 bits are address bits, last 3 bits are set to 011
  pml4e[vmem1] = (((uint64_t)pdpe) & 0xFFFFFFFFFF000) | 3;
  pdpe[vmem2]  = (((uint64_t)pde)  & 0xFFFFFFFFFF000) | 3;
  pde[vmem3]   = (((uint64_t)pte)  & 0xFFFFFFFFFF000) | 3;
  
  uint64_t i = 0xB8000; 
  pte[vmem4] = ((((uint64_t)i) & 0xFFFFFFFFFF000) | 3);
  cr3 = ((uint64_t)pml4e);
  // Setting the Page Tables
  _ptcr3(cr3); //setting cr3 register to kick start paging
}
*/
