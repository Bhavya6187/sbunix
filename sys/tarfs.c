#include <defs.h>
#include <stdio.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/isr.h>
#include <sys/irq.h>
#include <sys/phy_mem.h>
#include <sys/page_table.h>
#include <sys/tasking.h>
#include <sys/tarfs.h>

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
 
void read_pheader(char* addr)
{
   struct pheader* pbase = (struct pheader*)(addr);
   printf("name = %s\n",pbase->p_type);

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
void readelf(char* addr)
{
  struct elf_header* elf_base = (struct elf_header*)(addr);
  char* pheader;
  //printf("name = %s\n",elf_base->e_ident);
  //printf("name = %s\n",elf_base->e_type);
  //printf("name = %s\n",elf_base->e_machine);
  pheader = (addr)+size_offset(elf_base->e_phoff);

  //pheader = (addr)+(size_elf(elf_base->e_ehsize)+size_offset(elf_base->e_phoff));
  printf("\nsize of elf  = %ld  size of offset = %ld\n",size_elf(elf_base->e_ehsize), size_offset(elf_base->e_phoff));
  read_pheader(pheader);
  pheader = (addr)+size_offset(elf_base->e_phoff)+size_elf(elf_base->e_phentsize);
  read_pheader(pheader);
/*  for(int i=0;  i <= 24; i++)
  {
    printf("%d = %x ",i,elf_base[i]);
    //elf_base = elf_base+1;
  }*/
}

void read_tarfs(){
 //char* end_pos =(char*) (&_binary_tarfs_end);
  int temp_size; 
  struct posix_header_ustar *header =  (struct posix_header_ustar*)(&_binary_tarfs_start);
  struct posix_header_ustar *end =  (struct posix_header_ustar*)(&_binary_tarfs_end);
  
// while(header < (struct posix_header_ustar*)(&_binary_tarfs_end))
 while(header < end)
 {
    printf("name = %s\n",header->name);
    printf("size = %s\n",header->size);
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
    temp_size = size_to_int(header->size) ;
    printf("size given by function = %ld\n",temp_size);
    if(temp_size > 0)
    {

      printf("\nThe address with header %p", header);
      header = (header+1);
      printf("\nThe address with elf %p\n", header);
      readelf((char*)header) ;
      return;
    }
    //new_pos = new_pos + temp_size + sizeof(struct posix_header_ustar);
    //printf("header = %p, new_pos = %p, size of struct = %d\n",header, new_pos, sizeof(struct posix_header_ustar));
    header =  (struct posix_header_ustar*)((char*)header + sizeof(struct posix_header_ustar) + temp_size);
    //header =  (struct posix_header_ustar*)new_pos;
 }
}

