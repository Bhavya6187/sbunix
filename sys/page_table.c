#include <defs.h>
#include <stdio.h>
#include <sys/phy_mem.h>
#include <sys/page_table.h>

#define PAGE_SIZE 4096
extern void _ptcr0(uint64_t ); //set 32nd bit and enables paging
extern void _ptcr3(uint64_t ); //setting cr3 register to kick start paging


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
uint64_t set_paging(void * km, void * pf, void * pb)
{
  uint64_t kernmem, physfree, physbase;
  uint64_t *pml4e, *pdpe, *pde, *pte, *thor;
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
  thor = (uint64_t *)allocate_free_phy_page();
 
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
  pml4e[vmem1] = (((uint64_t)pdpe) & 0xFFFFFFFFFF000) | 7;
  pdpe[vmem2]  = (((uint64_t)pde)  & 0xFFFFFFFFFF000) | 7;
  pde[vmem3]   = (((uint64_t)pte)  & 0xFFFFFFFFFF000) | 7;
  pde[vmem3+1]   = (((uint64_t)thor)  & 0xFFFFFFFFFF000) | 7;
  pml4e[510] = (((uint64_t)pml4e)  & 0xFFFFFFFFFF000) | 7;
//  pml4e[509] = (((uint64_t)thor) & 0xFFFFFFFFFF000) | 7;

  //pml4e[vmem4] = (((uint64_t)pml4e)  & 0xFFFFFFFFFF000) | 3;

  uint64_t i; 
  int j=0;
  i=physbase;
  while(i<physfree)
  {
    if(j<511)
    pte[vmem4+j] = ((((uint64_t)i) & 0xFFFFFFFFFF000) | 3);
    else
    thor[(j%512)] = ((((uint64_t)i) & 0xFFFFFFFFFF000) | 3);
    i+=PAGE_SIZE;
    j++;
  }
  /*for(j=0,i=physbase; i<physfree; i+=PAGE_SIZE,j++ )
  {
    pte[vmem4+j] = ((((uint64_t)i) & 0xFFFFFFFFFF000) | 3);

  }*/
  printf("i = %p j=%d", i, j);
  printf("\n %d %d %d %d\n", vmem1, vmem2, vmem3, vmem4);
  /*pdpe_vm = (uint64_t *)allocate_free_phy_page();
  pde_vm = (uint64_t *)allocate_free_phy_page();
  pml4e[vmem1] = (((uint64_t)pdpe_vm) & 0xFFFFFFFFFF000) | 3;
  pdpe[vmem2]  = (((uint64_t)pde_vm)  & 0xFFFFFFFFFF000) | 3;
  pde[vmem3]   = (((uint64_t)pte_vm)  & 0xFFFFFFFFFF000) | 3;*/
  //pte_vm[vmem4] = ((((uint64_t)0xB8000000) & 0xFFFFFFFFFF000) | 3);
  //for(j=0, i=0xB8000; j<=0; i+=PAGE_SIZE,j++ )
    //pte_vm[vmem4+j] = ((((uint64_t)i) & 0xFFFFFFFFFF000) | 3);
  //pte_vm = (uint64_t *)allocate_free_phy_page();
  //pte[vmem4+j] = ((((uint64_t)0xB8000) & 0xFFFFFFFFFF000) | 3);
  if(j>511)
  thor[vmem4+(j%512)] = ((((uint64_t)0xB8000) & 0xFFFFFFFFFF000) | 3);
  else
  thor[vmem4+j] = ((((uint64_t)0xB8000) & 0xFFFFFFFFFF000) | 3);

//  pte[vmem4+j] = (0xB8000  | 3);
  //printf("\n %d %d %d %d %p %p %p\n", vmem1, vmem2, vmem3, vmem4, pml4e[vmem1], pte_vm[vmem4], video_mem);
  //printf("j=%d\n",j);
  //printf("%p %p %p %p %p %p %p %p\n", pml4e, pdpe, pde, pte, pdpe_vm, pde_vm, pte_vm);

  cr3 = ((uint64_t)pml4e);
  // Setting the Page Tables
  //printf("%x %x\n", head_fl, head_fl->next);
  vm_fl = 0xffffffff80000000;
//  _ptcr3(cr3); //setting cr3 register to kick start paging
	//head_fl->addr = 0xffffffff80000000 + physbase;
  // Mapping video memory
  /*free_list *ad = NULL;
  ad = (free_list*)((uint64_t)head_fl | 0xffffffff80000000 );*/
  //video_vm = (0xffffffff80000000|(uint64_t)physfree);
  //video_vm = (0xffffffff80000000|(uint64_t)physfree);
  video_vm = (0xffffffff80000000|(uint64_t)(physfree));

  _ptcr3(cr3); //setting cr3 register to kick start paging
  //printf("head=%x ad=%p \n", head_fl,  ad);
  //printf("adn:%p:\n", ad->next);
  return cr3;

}
/*
void page_mapping(uint64_t v_ad)
{
	int p1_va, p2_va, p3_va, p4_va;	// 9 bits each from v_kern for index into Page table hiearchy -- INDEX into 4 level page tables
	uint64_t add = 0xFFFF000000000000;	// Base address to build on
	uint64_t *p1, *p2, *p3, *p4, *p5;		// Pointers to each level Page table p1 == PML4E

	p1_va = (v_ad << 16) >> (9 + 9 + 9 + 12 + 16);	// Index in PML4E
	p2_va = (v_ad << (16 +9)) >> (9 + 9 + 12 + 16 + 9); // Index in PDPE
	p3_va = (v_ad << (16 + 9 + 9)) >> (9 + 12 + 16 + 9 + 9); // Index in PDE 
	p4_va = (v_ad << (16 + 9 + 9 + 9)) >> (12 + 16 + 9 + 9 + 9); //Index in PTE

  printf("\nIndex:p1:%d:p2:%d:p3:%d:p4:%d", p1_va, p2_va, p3_va, p4_va);
	Creating self looping for PML4E
// abhi Write a single function for this
	add = (((add >> 48) << 9 | 0x1FE) << 39);  // Sets the 1st 9 bits to 510 for  self refrenccing
	add = (((add >> 39) << 9 | 0x1FE) << 30);  // Sets the 2nd 9 bits to 510 for  self refrenccing
	add = (((add >> 30) << 9 | 0x1FE) << 21);  // Sets the 3rd 9 bits to 510 for  self refrenccing
	add = (((add >> 21) << 9 | 0x1FE) << 12);  // Sets the 4th 9 bits to 510 for  self refrenccing
	p1 = (uint64_t *)add;
	p2 = (uint64_t *)p1[p1_va];
	printf("\n ADD:p1:%p", add);

	if (!p2)
	{
		printf("\n p2 not found in p1:Allocating");
		p2 = (uint64_t *) allocate_free_phy_page();
		p1[p1_va] = ((((uint64_t)p2) & 0xFFFFFFFFFF000) | 3);
		printf("\n p2 not found in p1:AllocatED:%p", p2);
	}
	
	add = 0xFFFF000000000000;
	add = (((add >> 48) << 9 | 0x1FE) << 39);
	add = (((add >> 39) << 9 | 0x1FE) << 30);
	add = (((add >> 30) << 9 | 0x1FE) << 21);
	add = (((add >> 21) << 9 | p1_va) << 12);
	p2 = (uint64_t*)add;
	p3 = (uint64_t*)p2[p2_va];
	printf("\n ADD:p2:%p", add);

	if(!p3)
	{
		printf("\n p3 not found in p2:Allocating");
		p3 = (uint64_t *) allocate_free_phy_page();
		p2[p2_va] = ((((uint64_t) p3) & 0xFFFFFFFFFF000) | 3);
		printf("\n p3 not found in p2:AllocatED:%p", p3);
	}

	add = 0xFFFF000000000000; 
	add = (((add >> 48) << 9 | 0x1FE) << 39);
	add = (((add >> 39) << 9 | 0x1FE) << 30);
	add = (((add >> 30) << 9 | p1_va) << 21);
	add = (((add >> 21) << 9 | p2_va) << 12);
	p3 = (uint64_t*)add;
	p4 = (uint64_t*)p3[p3_va];
	printf("\n ADD:p3:%p", add);

	if(!p4)
	{
		printf("\n p4 not found in p3:Allocating");
		p4 = (uint64_t *) allocate_free_phy_page();
		p3[p3_va] = ((((uint64_t) p4) & 0xFFFFFFFFFF000) | 3);
		printf("\n p4 not found in p3:AllocatED:%p", p4);
	}


	add = 0xFFFF000000000000; 
	add = (((add >> 48) << 9 | 0x1FE) << 39);
	add = (((add >> 39) << 9 | p1_va) << 30);
	add = (((add >> 30) << 9 | p2_va) << 21);
	add = (((add >> 21) << 9 | p3_va) << 12);
	p4 = (uint64_t*)add;
	p5 = (uint64_t*)p4[p4_va];
	printf("\n ADD:p4:%p", add);
				
	if(!p5)
	{
		printf("\n p5 not found in p4:Allocating");
		p5 = (uint64_t *)allocate_free_phy_page();
		p4[p4_va] = ((((uint64_t) p5) & 0xFFFFFFFFFF000) | 3);
		printf("\n p5 not found in p4:AllocatED:%p", p5);
	}
	printf("ASTALAVISTA");
}
*/
void page_mapping(uint64_t vadd)
{
    int pml4eindex = getPML4Eindex(vadd);
    int pdpeindex = getPDPEindex(vadd);
    int pdeindex = getPDEindex(vadd);
    int pteindex = getPTEindex(vadd);
    uint64_t *pml4e=0, *pdpe=0, *pde=0, *pte=0, *paddr=0;
    //Read cr3 here -
  	//asm volatile(
		//"movq %%cr3, %0"
		//:"=g"(pml4e)
		//:
	  //:"memory"
	  //);
    uint64_t base1, base2, base3, base4;
    printf("pml4eindex= %d : pdpeindex = %d : pdeindex = %d : pteindex= %d \n", pml4eindex, pdpeindex, pdeindex, pteindex);
    //, setindices1, setindices2, setindices3, setindices4;
    printf("In page_mapping:: Virtual Address is = %p\n",vadd);
    // Access PML4E table
    base1 = 0xFFFF000000000000; 
    base1 = (((base1 >> (12+9+9+9+9))<<9 | 0x1FE ) << (12+9+9+9) );
    base1 = (((base1 >> (12+9+9+9))<<9 | 0x1FE   ) << (12+9+9) );
    base1 = (((base1 >> (12+9+9))<<9 | 0x1FE     ) << (12+9) );
    base1 = (((base1 >> (12+9))<<9 | 0x1FE       ) << (12) );
    pml4e = (uint64_t*)base1;
    pdpe = (uint64_t*)pml4e[pml4eindex];
    printf("base pdpe = %p %p : ", base1, pdpe);
    if(!pdpe)
    {
        pdpe = (uint64_t *)allocate_free_phy_page();
        pml4e[pml4eindex] = (((uint64_t)pdpe) & 0xFFFFFFFFFF000) | 7;
        printf("allocating pdpe = %p\n", pdpe);
    }
    
    // Access PDPE table
    base2 = 0xFFFF000000000000; 
    base2 = (((base2 >> (12+9+9+9+9))<<9 | 0x1FE ) << (12+9+9+9) );
    base2 = (((base2 >> (12+9+9+9))<<9 | 0x1FE   ) << (12+9+9) );
    base2 = (((base2 >> (12+9+9))<<9 | 0x1FE     ) << (12+9) );
    base2 = (((base2 >> (12+9))<<9 |pml4eindex   ) << (12) );
    pdpe = (uint64_t*)base2;
    pde = (uint64_t*)pdpe[pdpeindex];
    printf("base pde= %p %p : ", base2, pde);
    if(!pde)
    {
        pde = (uint64_t *)allocate_free_phy_page();
        pdpe[pdpeindex] = (((uint64_t)pde) & 0xFFFFFFFFFF000) | 7;
        printf("allocating pde = %p\n", pde);
    }

    // Access PDE table
    base3 = 0xFFFF000000000000; 
    base3 = (((base3 >> (12+9+9+9+9))<<9 | 0x1FE ) << (12+9+9+9) );
    base3 = (((base3 >> (12+9+9+9))<<9 | 0x1FE   ) << (12+9+9) );
    base3 = (((base3 >> (12+9+9))<<9 | pml4eindex) << (12+9) );
    base3 = (((base3 >> (12+9))<<9 |pdpeindex    ) << (12) );
    pde = (uint64_t*)base3;
    pte = (uint64_t*)pde[pdeindex];
    printf("base pte= %p %p : ", base3, pte);
    if(!pte)
    {
        pte = (uint64_t *)allocate_free_phy_page();
        pde[pdeindex] = (((uint64_t)pte) & 0xFFFFFFFFFF000) | 7;
        printf("allocating pte = %p\n", pte);
    }

    // Access PTE table
    base4 = 0xFFFF000000000000; 
    base4 = (((base4 >> (12+9+9+9+9))<<9 | 0x1FE   ) << (12+9+9+9) );
    base4 = (((base4 >> (12+9+9+9))<<9 | pml4eindex) << (12+9+9) );
    base4 = (((base4 >> (12+9+9))<<9 | pdpeindex   ) << (12+9) );
    base4 = (((base4 >> (12+9))<<9 |pdeindex       ) << (12) );
    pte = (uint64_t*)base4;
    paddr = (uint64_t*)pte[pteindex];
    printf("base paddr= %p %p %d : ", base4, paddr, pteindex);
    if(!paddr)
    {
        paddr = (uint64_t *)allocate_free_phy_page();
        pte[pteindex] = (((uint64_t)paddr) & 0xFFFFFFFFFF000) | 7;
        printf("allocating paddr = %p\n", paddr);
    }
    printf("Return from page_mapping\n");

}
