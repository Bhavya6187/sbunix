#include <defs.h>
#include <stdio.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/isr.h>
#include <sys/irq.h>
#include <sys/phy_mem.h>
#include <sys/page_table.h>
#include <sys/task_management.h>
#include <sys/tarfs.h>

int strcmp2(char* str1, char*str2)
{
  while(*str1 !='\0' || *str2!='\0' )
  {

    if(*str1 != *str2)
      return 0;
    str1++;
    str2++;
  }
  if(*str1=='\0'&&*str2=='\0')
  {
    return 1;
  }
  return 0;
}
char zero_size[12] = "000000000000";
int size_to_int(char* size)
{
  int ret = 0;
  int base = 1;
  int i =11;
  while(size[i] <= '0' || size[i] > '9'  )
    i--;
  for(i=i;  i >= 0; i--)
  {
    ret += base*(size[i] - '0');
    base*=8;
  }
  return ret;
}
 
int psize_to_int(char* size)
{
  int ret = 0;
  int base = 1;
  int i =7;
  while(size[i] <= '0' || size[i] > '9'  )
    i--;
  for(i=i;  i >= 0; i--)
  {
    ret += base*(size[i] - '0');
    base*=8;
  }
  return ret;
}
VMA* read_pheader(char* addr, struct elf_header* elf_base)
{
   struct pheader* pbase = (struct pheader*)(addr);
   uint64_t filesz,memsz,vaddr,offset;
   VMA* vma = NULL;
   filesz = pbase->p_filesz;//psize_to_int(pbase->p_filesz);
   memsz = pbase->p_memsz;
   vaddr = pbase->p_vaddr;
   offset = pbase->p_offset;
   uint64_t base = (uint64_t) elf_base;
   uint64_t contents = base + offset;

   printf("\n in read vadd = %p source =  %p f_size = %p m_size%p\n", vaddr, contents,filesz,memsz);
   if(memsz != 0)
   {
     uint32_t ret = m_map((uint64_t)vaddr,(uint64_t)contents, filesz, memsz);
     if(ret == 0)
   	   vma = create_vma(vaddr,memsz);
   }
   //printf("name = %s\n",pbase->p_type);
   return vma;

}

uint64_t size_elf(char *size)
{
  uint64_t header_size = ((uint64_t)(*size))+256*((uint64_t)(*(size+1)));
  return header_size;
}

int size_offset(char* size)
{
  uint64_t off = 0;
  uint64_t base = 1;
  for(int i = 0;i < 8 ; i++)
  {
    off+=base*((uint64_t)size[i]);
    base*=8;
  }
  return off;
}

int get_num_segs(char* num)
{
  return 10*((num[1]) ) + (num[0]);
}

void readelf(char* addr, PCB *task)
{
//  clear_screen();
  struct elf_header* elf_base = (struct elf_header*)(addr);
  char* pheader;
  //printf("ident = %s\n",elf_base->e_ident);
  //printf("type = %s\n",elf_base->e_type);
  //printf("e_machine = %s\n",elf_base->e_machine);
  pheader = (addr)+size_offset(elf_base->e_phoff);

  //pheader = (addr)+(size_elf(elf_base->e_ehsize)+size_offset(elf_base->e_phoff));
  //printf("\nsize of elf  = %d  size of offset = %d num_entries=%d %c %c\n",size_elf(elf_base->e_ehsize), size_offset(elf_base->e_phoff), get_num_segs(elf_base->e_phnum), (elf_base->e_phnum[0] - '0'), (elf_base->e_phnum[1]-'0') );
  int i = 0;
  int num_entries = get_num_segs(elf_base->e_phnum);
  task->rip = elf_base->e_entry;
  VMA* local = NULL;
  VMA* ht = NULL;

  for(i = 0; i < num_entries; i++ )
  {
    printf("reading entry %d out of %d entries\n",i,num_entries);
  	if (i == 0)
		{
			local = read_pheader(pheader, elf_base);
			task->mm_st = local; 
		 	pheader = pheader+size_elf(elf_base->e_phentsize);
		}	
		else 
		{
    	ht  = read_pheader(pheader, elf_base);
      if(ht!=NULL)
      {
  			local->vma_next = ht;
	  		local = local->vma_next;
		   	pheader = pheader+size_elf(elf_base->e_phentsize);
      }
    }
  }
}

int read_tarfs(PCB* task, char* name){
 //char* end_pos =(char*) (&_binary_tarfs_end);
  if(name == NULL)
    return 1;
  int temp_size;
  int step = 0,mod; 
  char* elf_header = NULL;
  char* prevname = NULL;
  char* prevsize = NULL;
  struct posix_header_ustar *header =  (struct posix_header_ustar*)(&_binary_tarfs_start);
  struct posix_header_ustar *end =  (struct posix_header_ustar*)(&_binary_tarfs_end);
  
// while(header < (struct posix_header_ustar*)(&_binary_tarfs_end))
 while(header < end)
 {
    printf("name = %s\n",header->name);
 //   printf("size = %s\n",header->size);
    /*printf("mode = %s\n",header->mode);
    printf("uid = %s\n",header->uid);
    printf("gid = %s\n",header->gid);
    printf("mtime = %s\n",header->mtime);
    printf("checksum = %s\n",header->checksum);
    printf("typeflag = %s\n",header->typeflag);
    printf("prefix = %s\n",header->prefix);
    printf("linkname = %s\n",header->linkname);
    printf("magic = %s\n",header->magic);
    printf("version = %s\n",header->version);
    printf("uname = %s\n",header->uname);
    printf("gname = %s\n",header->gname);
    printf("devmajor = %s\n",header->devmajor);
    printf("devminor = %s\n",header->devminor);
    printf("pad = %s\n",header->pad);*/
    temp_size = size_to_int(header->size);
    printf("size given by function = %d\n",temp_size);
    if(temp_size > 0 && strcmp2(name,header->name))
    {

        printf("\nThe address with header %p", header);
        printf("\nThe address with elf %p\n", header);
        printf("\nStrcmp says %d\n", strcmp2(name,header->name));
        elf_header =(char*)(header+1);
        readelf((char*)elf_header, task);
//        while(1);
        return 0;
    }
    
    if(header->name[0] == '\0' && header->size[0]=='\0' && prevname == '\0' && prevsize == '\0')
    {
        
      printf("\nfound consecutive 0\n");
      return 2;
    }

    //new_pos = new_pos + temp_size + sizeof(struct posix_header_ustar);
    //printf("header = %p, new_pos = %p, size of struct = %d\n",header, new_pos, sizeof(struct posix_header_ustar));
    mod = temp_size%(sizeof(struct posix_header_ustar));
    if(mod == 0)
      step = 1 +(int)( temp_size/(sizeof(struct posix_header_ustar)));
    else
      step = 2 +(int)( temp_size/(sizeof(struct posix_header_ustar)));
    prevname = header->name;
    prevsize = header->size;
    header = header+step;
    //header =  (struct posix_header_ustar*)((char*)header + sizeof(struct posix_header_ustar) + temp_size);
    //header =  (struct posix_header_ustar*)new_pos;
 }
 return 2;
}

