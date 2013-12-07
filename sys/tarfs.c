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
#include <sys/string.h>
#include <sys/errno.h>

int fd_used = 2;
FDT fd_table[150];

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

int power(int x, int y)
{
   if( y == 0)
     return 1;
   else if (y%2 == 0)
     return power(x, y/2)*power(x, y/2);
   else
     return x*power(x, y/2)*power(x, y/2);
               
}

int size_to_int(char* p)
{
    int k = 0;
   while (*p) {
    k = (k<<3)+(k<<1)+(*p)-'0';
    p++;
  }
  int decimal=0, i=0, rem; 
  while (k!=0) 
  { 
     rem = k%10; 
     k/=10; 
     decimal += rem*power(8,i); 
     ++i;
  }
      
      return decimal;
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

struct File* getFile(const char *fileName)
{
      struct posix_header_ustar * entry = (struct posix_header_ustar*)(&_binary_tarfs_start);
      int padding =0;
      errno = 0;
      struct File* file = (struct File *)k_malloc(sizeof(struct File));
      int exitflag = 0;
      while((uint64_t)entry < (uint64_t)&_binary_tarfs_end)
      {
        int size = size_to_int(entry->size);
        if(strcmp(entry->typeflag,"5")  == 0 && strncmp(fileName,entry->name,lastIndexOf (fileName, "/")+1) == 0 &&  lastIndexOf (fileName, "/")+ 1 == strlen(entry->name)){
        strncpy(file->parent.d_name,entry->name,NAMEMAX);
        file->parent.offset = (char*)entry - (char*)&_binary_tarfs_start;
        if(++exitflag >= 2)
            break;
         }
        if(strcmp(fileName,entry->name) == 0 && strcmp(entry->typeflag,"0")  == 0){
        strncpy(file->path, fileName, NAMEMAX);
        file->offset = (char*)entry - (char*)&_binary_tarfs_start;
        if(++exitflag >= 2)
                    break;
        }
        entry = (struct posix_header_ustar *)((char*)entry + sizeof(struct posix_header_ustar) + size );
        if(size > 0){
          padding = BLOCKSIZE - size%BLOCKSIZE;
          if((char*)&_binary_tarfs_end - (char*)entry >=BLOCKSIZE)
          {
            entry = (struct posix_header_ustar *)((char*)entry + padding);
          }
        }
      }

      if(strcmp(fileName,entry->name)!=0){
        return NULL;
      }
      return file;
}

uint64_t read_file(uint64_t fd,uint64_t num, char* buffer)
{
  volatile uint64_t i = 0;
  while(fd_table[i].number != fd && i <150)
  {
      i++;
  }
  if(i >=150)
    return NULL;

  //printf("\nFile has fd %d",fd);
  FDT entry = fd_table[i];
  char* base = (char*)&_binary_tarfs_start;
  base = base + entry.fp->offset ;
  struct posix_header_ustar* tarfs_struct = (struct posix_header_ustar*)base;
  int size = size_to_int(tarfs_struct->size);
  if(size == 0)
    return NULL;
  //printf("size of file being read  = %d with seek = %d\n",size,entry.seek);
  base = (char*)(tarfs_struct+1);
  base = base + entry.seek ;

  int index = 0;
  for(index=0; index < num ; index++ )
  {
    if(entry.seek<size)
    {
      buffer[index] = base[index]; 
      entry.seek++;
    }
    else
    {
      fd_table[i].seek = entry.seek;
      return -2;
    }
  }
  fd_table[i].seek = entry.seek;
  return i;
}

uint64_t close_file(uint64_t fd)
{
  uint64_t i = 0;
  while(fd_table[i].number != fd && i<150)
  {
      i++;
  }
  if(i >=150)
    return 1;
  fd_table[i].number=-1;
  fd_table[i].fp=NULL;
  fd_table[i].seek = 0;
  return 0;
}

uint64_t open_file(char* filename)
{
  uint64_t i = 0;
  while(fd_table[i].number != -1)
  {
      i++;
  }
  if(getFile(filename)==NULL)
    return -1;
  fd_table[i].number=fd_used + 1;
  fd_table[i].fp=getFile(filename) ;
  if(fd_table[i].fp == NULL)
    return -1;
  fd_table[i].seek = 0;
  return fd_table[i].number;
}
